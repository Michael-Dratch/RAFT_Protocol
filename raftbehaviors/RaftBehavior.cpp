//
// Created by Owner on 6/4/2023.
//
#include <sys/socket.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/un.h>
#include "RaftBehavior.h"
#include "../Serialization.h"


void RaftBehavior::sendAppendEntriesResponse(RaftMessage message, bool success) {
    int socket = createSocket();
    connectToServer(getServerAddrFromID(message.senderID), socket);
    RaftMessage msg = createRaftMessage(2, success, server->serverID, 0,0,0,0, 0,0,0,"");
    sendRaftMessage(socket, msg);
    close(socket);
}

void RaftBehavior::connectToServer(sockaddr_in addr, int socket) {
    int ret;
    ret = connect (socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_in));
    checkError(ret, "Server: The server is down");
}

int RaftBehavior::createSocket() {
    int data_socket = socket(AF_INET, SOCK_STREAM, 0);
    checkError(data_socket, "creating socket");
    return data_socket;
}

void RaftBehavior::sendRaftMessage(int socket, RaftMessage msg) {
    uint8_t packet[37];
    serializeMessage(msg, packet);
    int entriesSize = msg.entries.size();
    uint8_t entriesPacket[entriesSize];
    serializeEntries(msg, entriesPacket);
    sendPacket(socket, packet, 37);
    sendPacket(socket, entriesPacket, entriesSize);
}

void RaftBehavior::sendPacket(int socket, uint8_t *packet, int packetSize) {
    int ret = send(socket, packet, packetSize, 0);
    if (ret == -1){std::cerr << "Error: Failed to send data. Error code: " << errno << std::endl;}
}

void RaftBehavior::printLog() {
    cout << "SERVER CURRENT LOG " << endl;
    for (int i = 0; i < server->log.size(); i++) {
        cout << "TERM: " << server->log.at(i).term << " VALUE: " << server->log.at(i).value << endl;
    }
}

sockaddr_in RaftBehavior::getServerAddrFromID(int id) {
    //Server IDs are the index of that servers address in the address vector
    return server->serverAddresses[id];
}

void RaftBehavior::checkError(int ret, string message) {
    if (ret == -1) {
        cout << "ERROR: " << message << endl;
        exit(EXIT_FAILURE);
    }
}

void RaftBehavior::updateCurrentTerm(int term) {
    if (term > server->currentTerm){ server->currentTerm = term;}
}