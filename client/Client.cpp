
#include "Client.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/un.h>
#include "../RaftMessage.h"


using namespace std;

/*
 *  This function "starts the client".  It takes a header structure
 *  and a properly formed packet.  The packet is basically the
 *  header with a message at the end of it in the case of the
 *  command being CMD_PING_PONG.  The length in the header field
 *  indicates the total size to send to the server, thus it will
 *  be sizeof(cs472_proto_header_t) in the case that the cmd is
 *  CMD_COURSE_INFO, or it will be 
 *  sizeof(cs472_proto_header_t) + strlen(MESSAGE) + 1 in the case
 *  we are doing a ping to the server.  We add the extra 1 to send
 *  over the null terminator for the string
 */

void Client::start_client(uint8_t *packet){
    struct sockaddr_in addr;
    int data_socket, ret;

    data_socket = createSocket(data_socket);
    clearMemory(addr);

    connectToServer(PORT_NUM, data_socket);

    ret = send(data_socket, packet, strlen((const char *)packet), 0);
    checkError(ret, "header write error");

    //NOW READ RESPONSES BACK - 2 READS, HEADER AND DATA
    ret = recv(data_socket, recv_buffer, sizeof(recv_buffer),0);
    checkError(ret, "read error");

    printf("RECV FROM SERVER -> %s\n",recv_buffer);

    close(data_socket);
}

void Client::start_client(vector<sockaddr_in> serverAddrs, vector<RaftMessage> messages){
    serverAddresses = serverAddrs;
    messageQueue = messages;
    int data_socket, ret;

    data_socket = createSocket(data_socket);

    for (int msgIndex = 0; msgIndex < messageQueue.size(); msgIndex++){
        RaftMessage msg = messageQueue[msgIndex];
        vector<uint8_t> packet = msg.serialize();
        connectToServer(serverAddrs[0], data_socket);
        sendMessage(data_socket, packet);
    }
    close(data_socket);
}

void Client::sendMessage(int data_socket, vector<uint8_t> &packet) {
    int ret;
    ret = send(data_socket, &packet, packet.size(), 0);
    checkError(ret, "header write error");
}


void Client::clearMemory(sockaddr_in &addr) {
    memset(&addr, 0, sizeof(struct sockaddr_in));
}

int Client::createSocket(int data_socket) {
    data_socket = socket(AF_INET, SOCK_STREAM, 0);
    checkError(data_socket, "creating socket");
    return data_socket;
}

void Client::connectToServer(unsigned short port, int data_socket) {
    struct sockaddr_in addr;
    int ret;
    clearMemory(addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(port);

    ret = connect (data_socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_in));
    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }
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

