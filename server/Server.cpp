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
#include <cstdint>
#include "Server.h"
#include "../Serialization.h"
#include "../RaftMessage.h"

using namespace std;

void Server::process_requests(int listen_socket) {
    int data_socket, bytesRead;
    clearBuffers();
    data_socket = acceptConnection(listen_socket, data_socket);

    bytesRead = recv(data_socket, recv_buffer, 37, 0);
    checkError(bytesRead, "read error");
    RaftMessage msg = deserializeMessage(recv_buffer);
    bytesRead = recv(data_socket, recv_buffer, msg.entriesLength, 0);
    char* entries = (char*)&recv_buffer[37];
    cout << entries << endl;
    msg.entries = entries;
    cout << msg.entries << endl;
    cout << "Server: Message: " << raftMessageToString(msg) << endl;
    close(data_socket);
}

//int Server::deserializeInt(uint8_t*bufferPtr) {
//    uint8_t intData[4];
//    for (int i = 0; i < 4; i++) {
//        intData[i] = bufferPtr[i];
//    }
//    return (int) *intData;
//}


//
//    struct RaftMessage msg;
//    vector<uint8_t> data;
//    while ( bytesRead = recv(data_socket, recv_buffer, 4, 0) != 0) {
//        checkError(bytesRead, "read error");
//        for (int i = 0; i < bytesRead; i++){
//            data.push_back(recv_buffer[i]);
//        }
//        int* typePtr = (int*)(&data[0]);
//        int type = *typePtr;
//        cout << "TYPE: " << type << endl;
//    }
//    int entriesLength = recv_buffer[getEntriesLengthOffset(msg)];
//    cout << "Entires length: " << entriesLength << endl;
//    bytesRead = recv(data_socket, &recv_buffer[getHeaderSize(msg)], entriesLength, 0);
//    checkError(bytesRead, "read error");
//    deserializeRaftMessage(msg, recv_buffer);
//    cout << "RECEIVED MESSAGE" << endl;
//    cout << raftMessageToString(msg) << endl;
//  close(data_socket);
//    }

int Server::acceptConnection(int listen_socket, int data_socket) {
    data_socket = accept(listen_socket, NULL, NULL);
    checkError(data_socket, "accept");
    return data_socket;
}

void Server::clearBuffers() {
    memset(send_buffer, 0, sizeof(send_buffer));
    memset(recv_buffer, 0, sizeof(recv_buffer));
}


void Server::start_server(){
    cout << "SERVER: STARTED" << endl;
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





