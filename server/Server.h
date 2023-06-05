/*
 * THE MOST BASIC SOCKET SERVER
 *
 */



#include <cstdint>

#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/un.h>
#include "../RaftHost.h"
#include "../RaftMessage.h"
#include "../Entry.h"
#include "../raftbehaviors/RaftBehavior.h"
#include "../raftbehaviors/Follower.h"



#ifndef RAFT_PROTOCOL_SERVER_H
#define RAFT_PROTOCOL_SERVER_H


#define BUFF_SZ 512
#define CLIENT_MSG_TYPE 253
#define SHUTDOWN_MSG_TYPE 255
#define TEST_MSG_TYPE 254
#define APPEND_ENTRIES_TYPE 1
#define APPEND_ENTRIES_RES_TYPE 2
#define REQUEST_VOTE_TYPE 4
#define REQuEST_VOTE_RES_TYPE 8
#define FOLLOWER 1
#define LEADER 2


class Server: public RaftHost{

public:

    void start_server(int portNumber, vector<sockaddr_in> serverAddresses, int behaviorType);

    void process_requests(int listen_socket);

private:

    uint8_t send_buffer[BUFF_SZ];

    uint8_t recv_buffer[BUFF_SZ];

    RaftBehavior *behavior;

    void dispatch(RaftMessage message);

    void checkError(int ret, std::string message);

    void setSocketReuseOption(int listen_socket);

    void bindSocket(int listen_socket, int port, sockaddr_in &addr);

    void clearBuffers();

    int acceptConnection(int listen_socket);

    RaftMessage receiveMessageHeader(int data_socket);

    RaftMessage &receiveEntries(int data_socket, RaftMessage &msg);

    RaftMessage receiveRaftMessage(int data_socket);

    int createSocket();

    void connectToServer(sockaddr_in in, int socket);

    int getServerID(int portNumber);

    void sendPacket(int socket, uint8_t packet[37], int i);

    void handleTestMessage(RaftMessage message);

    void initializeRaftState();
};

#endif