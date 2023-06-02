
#include "Client.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/un.h>
#include <cstdint>
#include "../RaftMessage.h"
#include "../Serialization.h"


using namespace std;


void Client::start_client(vector<sockaddr_in> serverAddrs, vector<RaftMessage> messages){
    int data_socket;

    for (int i = 0; i < messages.size(); i++){
        data_socket = createSocket(data_socket);
        connectToServer(serverAddrs[0], data_socket);
        RaftMessage msg = messages[i];
        sendRaftMessage(data_socket, msg);
        close(data_socket);
    }

}

void Client::sendRaftMessage(int data_socket, RaftMessage &msg) {
    uint8_t packet[37];
    serializeMessage(msg, packet);
    int entriesSize = msg.entries.size();
    uint8_t entriesPacket[entriesSize];
    serializeEntries(msg, entriesPacket);
    sendPacket(data_socket, packet, 37);
    sendPacket(data_socket, entriesPacket, entriesSize);
}


void Client::sendPacket(int data_socket, uint8_t *packet, int packetSize) {
    int ret = send(data_socket, packet, packetSize, 0);
    if (ret == -1){std::cerr << "Error: Failed to send data. Error code: " << errno << std::endl;}
}

int Client::createSocket(int data_socket) {
    data_socket = socket(AF_INET, SOCK_STREAM, 0);
    checkError(data_socket, "creating socket");
    return data_socket;
}

void Client::connectToServer(sockaddr_in addr, int data_socket) {
    int ret;
    ret = connect (data_socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_in));
    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }
}

void Client::checkError(int ret, string message) {
    if (ret == -1) {
        cout << "Error: " << message << endl;
        exit(EXIT_FAILURE);
    }
}

