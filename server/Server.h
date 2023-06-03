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
#include "../RaftMessage.h"


#define BUFF_SZ 512

#define SHUTDOWN_MSG_TYPE 255
#define TEST_MSG_TYPE 254
#define APPEND_ENTRIES_TYPE 1
#define APPEND_ENTRIES_RES_TYPE 2
#define REQUEST_VOTE_TYPE 4
#define REQuEST_VOTE_RES_TYPE 8

class Server{

public:
    void start_server(int portNumber, vector<sockaddr_in> serverAddresses);

    void process_requests(int listen_socket);

private:
    uint8_t send_buffer[BUFF_SZ];

    uint8_t recv_buffer[BUFF_SZ];

    vector<sockaddr_in> serverAddresses;

    int serverID;

    int currentTerm;

    sockaddr_in votedFor;

    vector<string> log;

    int commitIndex;

    int lastApplied;

    void checkError(int ret, std::string message);

    void setSocketReuseOption(int listen_socket);

    void bindSocket(int listen_socket, int port, sockaddr_in &addr);

    void clearBuffers();

    int acceptConnection(int listen_socket);


    RaftMessage receiveMessageHeader(int data_socket);

    RaftMessage &receiveEntries(int data_socket, RaftMessage &msg);

    RaftMessage receiveRaftMessage(int data_socket);

    void dispatch(RaftMessage message);

    void handleAppendEntries(RaftMessage message);

    void sendAppendEntriesResponse(RaftMessage message, bool success);

    int createSocket();

    sockaddr_in getServerAddrFromID(int id);

    void connectToServer(sockaddr_in in, int socket);

    int getServerID(int portNumber);

    void sendRaftMessage(int socket, RaftMessage message);

    void sendPacket(int socket, uint8_t packet[37], int i);

    void updateCurrentTerm(int term);

    bool doesAppendEntriesFail(RaftMessage message);

    void handleTestMessage(RaftMessage message);
};