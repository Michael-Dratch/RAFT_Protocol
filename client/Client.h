//
// Created by Owner on 5/28/2023.
//

#ifndef RAFT_PROTOCOL_CLIENT_H
#define RAFT_PROTOCOL_CLIENT_H
#include <stdint.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include "../RaftMessage.h"

#define PORT_NUM    1090
#define BUFF_SZ 512
#define SHUTDOWN_MSG_TYPE 255

class Client {
public:
    void start_client(vector<sockaddr_in> serverAddresses, vector<RaftMessage> messageQueue);
    void connectToServer(unsigned short port, int data_socket);

private:
    uint8_t send_buffer[BUFF_SZ] = {0};
    uint8_t recv_buffer[BUFF_SZ] = {0};
    vector<sockaddr_in> serverAddresses;
    vector<struct RaftMessage> messageQueue;

    void checkError(int data_socket, std::string message);

    int createSocket(int data_socket);

    void connectToServer(sockaddr_in addr, int data_socket);

    void sendPacket(int data_socket, uint8_t*packet, int packetSize);

    void sendRaftMessage(int data_socket, RaftMessage &msg);
};


#endif //RAFT_PROTOCOL_CLIENT_H
