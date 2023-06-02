//
// Created by Owner on 5/26/2023.
//


#include <sys/socket.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <arpa/inet.h>
#include <cstdio>
#include <unistd.h>
#include <sys/un.h>
#include "Server.h"
#include "../RaftMessage.h"

using namespace std;

void Server::process_requests(int listen_socket){
    int data_socket, ret;
    clearBuffers();
    data_socket = acceptConnection(listen_socket, data_socket);

    ret = recv(data_socket, recv_buffer, RaftMessage::getHeaderSize(), 0);
    checkError(ret, "read error");

    int entriesLength = RaftMessage::deserializeInt(&recv_buffer[RaftMessage::getEntriesLengthOffset()]);

    ret = recv(data_socket, recv_buffer, entriesLength, 0);

    vector<uint8_t> msgData;
    for (int i = 0; i < RaftMessage::getHeaderSize() + entriesLength; i++){
        msgData.push_back(recv_buffer[i]);
    }

    RaftMessage message;
    message.deserialize(msgData);
    cout << message.toString() << endl;
    close(data_socket);
    }

int Server::acceptConnection(int listen_socket, int data_socket) {
    data_socket = accept(listen_socket, NULL, NULL);
    checkError(data_socket, "accept");
    printf("\t RECEIVED REQUEST\n");
    return data_socket;
}

void Server::clearBuffers() {
    memset(send_buffer, 0, sizeof(send_buffer));
    memset(recv_buffer, 0, sizeof(recv_buffer));
}


void Server::start_server(){
    int listen_socket, ret;
    struct sockaddr_in addr;

    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    checkError(listen_socket, "socket");
    setSocketReuseOption(listen_socket, ret);
    bindSocket(listen_socket, ret, addr);
    ret = listen(listen_socket, 20);
    checkError(ret, "listen");

    process_requests(listen_socket);
    close(listen_socket);
}

void Server::bindSocket(int listen_socket, int ret, sockaddr_in &addr) {
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT_NUM);

    ret = bind(listen_socket, (const struct sockaddr *) &addr,
               sizeof(struct sockaddr_in));
    checkError(ret, "accept");
}

void Server::setSocketReuseOption(int listen_socket, int ret) {
    int reuse = 1;
    ret = setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    checkError(ret, "SO_REUSEADDR option");
}

void Server::checkError(int ret, string message) {
    if (ret == -1) {
        cout << "ERROR: " << message << endl;
        exit(EXIT_FAILURE);
    }
}





