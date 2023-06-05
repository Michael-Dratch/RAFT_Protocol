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

    Server* self = this;
    Follower follower(self);
    behavior = &follower;

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
            behavior->handleAppendEntries(message);
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

int Server::getServerID(int portNumber) {
    for (int i = 0; i < serverAddresses.size(); i++){
        if (serverAddresses[i].sin_port == htons(portNumber)){ return i; }
    }
    cerr << "server address are misconfigured" << endl;
    exit(EXIT_FAILURE);
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












