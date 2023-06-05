//
// Created by Owner on 5/26/2023.
//


#include <sys/socket.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include "Server.h"
#include "../Entry.h"
#include "../Serialization.h"
#include "../RaftMessage.h"
#include "../raftbehaviors/Follower.h"


using namespace std;


void Server::start_server(int portNumber, vector<sockaddr_in> serverAddrs){
    cout << "SERVER: STARTED" << endl;
    serverAddresses = serverAddrs;
    serverID = getServerID(portNumber);
    initializeRaftState();
    int listen_socket, ret;
    struct sockaddr_in addr;

    //behavior = Follower(this);

    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    checkError(listen_socket, "socket");
    setSocketReuseOption(listen_socket);
    bindSocket(listen_socket, portNumber, addr);
    ret = listen(listen_socket, 20);
    checkError(ret, "listen");

    process_requests(listen_socket);
    close(listen_socket);
    cout << "SERVER: SHUTTING DOWN" << endl;
    exit(0);
}

void Server::process_requests(int listen_socket) {
    int data_socket, bytesRead;
    while(1){
        clearBuffers();
        data_socket = acceptConnection(listen_socket);
        RaftMessage msg = receiveRaftMessage(data_socket);
        close(data_socket);
        if (msg.type == SHUTDOWN_MSG_TYPE){
            cout << "SERVER SHUTTING DOWN" << endl;
            break;
        }
        dispatch(msg);
    }
}

void Server::dispatch(RaftMessage message) {
    switch(message.type){
        case APPEND_ENTRIES_TYPE:
            cout << "SERVER " << serverID << " RECEIVED APPEND ENTRIES MESSAGE" << endl;
            handleAppendEntries(message);
            break;
        case APPEND_ENTRIES_RES_TYPE:
            cout << "SERVER " << serverID << " RECEIVED APPEND ENTRIES RESPONSE" << endl;
            break;
        case REQUEST_VOTE_TYPE:
            cout << "SERVER " << serverID << " RECEIVED REQUEST VOTE" << endl;
            break;
        case REQuEST_VOTE_RES_TYPE:
            cout << "SERVER " << serverID << " RECEIVED REQUEST VOTE RESPONSE" << endl;
            break;
        case TEST_MSG_TYPE:
            handleTestMessage(message);
            break;
        default:
            return;
    }
}

void Server::handleAppendEntries(RaftMessage message) {
    updateCurrentTerm(message.currentTerm);
    if (doesAppendEntriesFail(message)){
        cout << "APPEND ENTRIES FAILS" << endl;
        sendAppendEntriesResponse(message, false);
        return;
    }
    cout << "APPEND ENTRIES SUCCEEDS" << endl;
    addEntriesToLog(message);
    sendAppendEntriesResponse(message, true);
    printLog();
}

bool Server::doesAppendEntriesFail(RaftMessage msg) {
    if (msg.currentTerm < currentTerm) {return true;}
    else if (msg.prevLogIndex == 0) {return false;}
    else if (log.size() < msg.prevLogIndex) {return true;}
    if (log.size() < 1) {return false;}
    else if( log.at(msg.prevLogIndex-1).term != msg.prevLogTerm) {return true;}
    return false;
}


void Server::sendAppendEntriesResponse(RaftMessage message, bool success) {
    int socket = createSocket();
    connectToServer(getServerAddrFromID(message.senderID), socket);
    RaftMessage msg = createRaftMessage(2, success, serverID, 0,0,0,0, 0,0,0,"");
    sendRaftMessage(socket, msg);
    close(socket);
}



void Server::connectToServer(sockaddr_in addr, int socket) {
    int ret;
    ret = connect (socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_in));
    checkError(ret, "Server: The server is down");
}

int Server::createSocket() {
    int data_socket = socket(AF_INET, SOCK_STREAM, 0);
    checkError(data_socket, "creating socket");
    return data_socket;
}

void Server::sendRaftMessage(int socket, RaftMessage msg) {
    uint8_t packet[37];
    serializeMessage(msg, packet);
    int entriesSize = msg.entries.size();
    uint8_t entriesPacket[entriesSize];
    serializeEntries(msg, entriesPacket);
    sendPacket(socket, packet, 37);
    sendPacket(socket, entriesPacket, entriesSize);
}

void Server::sendPacket(int socket, uint8_t *packet, int packetSize) {
    int ret = send(socket, packet, packetSize, 0);
    if (ret == -1){std::cerr << "Error: Failed to send data. Error code: " << errno << std::endl;}
}



RaftMessage Server::receiveRaftMessage(int data_socket) {
    RaftMessage msg = receiveMessageHeader(data_socket);
    msg = receiveEntries(data_socket, msg);
    return msg;
}

RaftMessage &Server::receiveEntries(int data_socket, RaftMessage &msg) {
    int bytesRead = recv(data_socket, recv_buffer, msg.entriesLength, 0);
    checkError(bytesRead, "Reading entries");
    msg.entries = deserializeEntries(recv_buffer, msg.entriesLength);
    return msg;
}


RaftMessage Server::receiveMessageHeader(int data_socket) {
    int bytesRead = recv(data_socket, recv_buffer, 37, 0);
    checkError(bytesRead, "read error");
    RaftMessage msg = deserializeMessage(recv_buffer);
    return msg;
}

int Server::acceptConnection(int listen_socket) {
    int data_socket = accept(listen_socket, NULL, NULL);
    checkError(data_socket, "accept");
    return data_socket;
}


void Server::clearBuffers() {
    memset(send_buffer, 0, sizeof(send_buffer));
    memset(recv_buffer, 0, sizeof(recv_buffer));
}

void Server::bindSocket(int listen_socket, int port, sockaddr_in &addr) {
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    int ret = bind(listen_socket, (const struct sockaddr *) &addr,
               sizeof(struct sockaddr_in));
    checkError(ret, "accept");
}

void Server::setSocketReuseOption(int listen_socket) {
    int reuse = 1;
    int ret = setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    checkError(ret, "SO_REUSEADDR option");
}

void Server::checkError(int ret, string message) {
    if (ret == -1) {
        cout << "ERROR: " << message << endl;
        exit(EXIT_FAILURE);
    }
}

sockaddr_in Server::getServerAddrFromID(int id) {
    //Server IDs are the index of that servers address in the address vector
    return serverAddresses[id];
}

int Server::getServerID(int portNumber) {
    for (int i = 0; i < serverAddresses.size(); i++){
        if (serverAddresses[i].sin_port == htons(portNumber)){ return i; }
    }
    cerr << "server address are misconfigured" << endl;
    exit(EXIT_FAILURE);
}

void Server::updateCurrentTerm(int term) {
    if (term > this->currentTerm){ currentTerm = term;}
}

void Server::handleTestMessage(RaftMessage message) {
    cout << "SETTING SERVER STATE" << endl;
    currentTerm = message.currentTerm;
}

void Server::initializeRaftState() {
    currentTerm = 0;
    commitIndex = 0;
    lastApplied = 0;
}

void Server::addEntriesToLog(RaftMessage msg) {
    string entries = msg.entries;
    int charIndex = 0;
    bool doneParsingEntries = false;
    if (msg.entriesLength <= 0){ doneParsingEntries = true;}
    while(!doneParsingEntries){
        Entry entry = parseEntry(entries, charIndex);
        log.push_back(entry);
        if (charIndex >= msg.entriesLength){
            doneParsingEntries = true;
        }
    }
}

Entry Server::parseEntry(string &entries, int &charIndex) {
    int term = parseTerm(entries, charIndex);
    string value = parseValue(entries, charIndex);
    Entry entry;
    entry.term = term;
    entry.value = value;
    return entry;
}

int Server::parseTerm(string &entries, int &i) {
    string termString = "";
    while (entries.at(i) != ' '){
        termString += entries.at(i);
        i++;
    }
    i++;
    int term;
    try {
        term = stoi(termString);
    } catch (const std::invalid_argument& e){
        cerr << "invalid argument exception: " << e.what() << endl;
        term = 0;
    }
    return term;
}

string Server::parseValue(string &entries, int &i) {
    string value= "";
    while(!(entries.at(i) == '\n' && entries.at(i + 1) == '\r')){
        value += entries.at(i);
        i++;
    }
    i += 2;
    return value;
}

void Server::printLog() {
    cout << "SERVER CURRENT LOG " << endl;
    for (int i = 0; i < log.size(); i++){
        cout << "TERM: " << log.at(i).term << " VALUE: " << log.at(i).value << endl;
    }
}












