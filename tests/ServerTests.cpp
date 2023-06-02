//
// Created by Owner on 5/28/2023.
//

// Work on creating test library for servers
// way of tracking what messages have been recieved by servers and clients

#include <iostream>
#include <gtest/gtest.h>
#include <string>
#include "../server/Server.h"
#include "../client/Client.h"
#include "../Serialization.h"
#define SHUTDOWN_MSG_TYPE 255
#define BUFF_SZ 512

using namespace std;


void waitForAllProcesses(vector<pid_t> pids);
void runServer();
void runClient();

int probeSendSocket, probeListenSocket;
uint8_t send_buffer[BUFF_SZ];
uint8_t recv_buffer[BUFF_SZ];

void runClient(vector<sockaddr_in> serverAddrs, vector<RaftMessage> &messages) {
    Client c;
    c.start_client(serverAddrs, messages);
    exit(0);
}

void runServer(int port, vector<sockaddr_in> serverAddrs) {
    Server s;
    s.start_server(port, serverAddrs);
    exit(0);
};

void waitForAllProcesses(vector<pid_t> pids){
    int pidCount = pids.size();
    for (int i = 0; i < pidCount; i++){
        waitpid(pids[i], NULL, 0);
    }
}

pid_t startServer(int port, vector<sockaddr_in> serverAddrs){
    pid_t serverPID = fork();
    if (serverPID == 0) {runServer(port, serverAddrs);}
    return serverPID;
}

pid_t startClient(vector<sockaddr_in> serverAddrs, vector<struct RaftMessage> &messages){
    pid_t clientPID = fork();
    if (clientPID == 0) {
        runClient(serverAddrs, messages); }
    return clientPID;
}

void clearMemory(sockaddr_in &addr) {
    memset(&addr, 0, sizeof(struct sockaddr_in));
}

struct sockaddr_in createSocketAddress(unsigned short port){
    struct sockaddr_in addr;
    clearMemory(addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(port);
    return addr;
}

void checkError(int ret, string message) {
    if (ret == -1) {
        cerr << "Error:" <<message << errno << endl;
        exit(EXIT_FAILURE);
    }
}

int createSocket() {
    int data_socket = socket(AF_INET, SOCK_STREAM, 0);
    checkError(data_socket, "creating socket");
    return data_socket;
}

void connectToServer(sockaddr_in addr, int data_socket) {
    int ret;
    ret = connect (data_socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_in));
    if (ret == -1) {
        cout << "probe: server is down" <<endl;
        fprintf(stderr, "Probe: The server is down.\n");
        exit(EXIT_FAILURE);
    }
}

void sendPacket(int data_socket, uint8_t *packet, int packetSize) {
    int ret = send(data_socket, packet, packetSize, 0);
    if (ret == -1){std::cerr << "Error: Failed to send data. Error code: " << errno << std::endl;}
}

void sendRaftMessage(int data_socket, RaftMessage &msg) {
    uint8_t packet[37];
    serializeMessage(msg, packet);
    int entriesSize = msg.entries.size();
    uint8_t entriesPacket[entriesSize];
    serializeEntries(msg, entriesPacket);
    sendPacket(data_socket, packet, 37);
    sendPacket(data_socket, entriesPacket, entriesSize);
}

void setSocketReuseOption(int listen_socket) {
    int reuse = 1;
    int ret = setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    checkError(ret, "SO_REUSEADDR option");
}

sockaddr_in createAddr(int port){
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    return addr;
}

void bindSocket(int listen_socket, int port) {
    sockaddr_in addr = createAddr(port);
    int ret = bind(listen_socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_in));
    checkError(ret, "bind socket");
}

void bindSocket(int listen_socket, sockaddr_in addr) {
    int ret = bind(listen_socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_in));
    checkError(ret, "accept");
}

int acceptConnection(int listen_socket) {
    int data_socket = accept(listen_socket, NULL, NULL);
    checkError(data_socket, "accept");
    return data_socket;
}

RaftMessage receiveEntries(int data_socket, RaftMessage &msg) {
    int bytesRead = recv(data_socket, recv_buffer, msg.entriesLength, 0);
    checkError(bytesRead, "Reading entries");
    msg.entries = deserializeEntries(recv_buffer, msg.entriesLength);
    return msg;
}

RaftMessage receiveMessageHeader(int data_socket) {
    int bytesRead = recv(data_socket, recv_buffer, 37, 0);
    checkError(bytesRead, "read error");
    RaftMessage msg = deserializeMessage(recv_buffer);
    return msg;
}


RaftMessage receiveRaftMessage(int data_socket) {
    RaftMessage msg = receiveMessageHeader(data_socket);
    msg = receiveEntries(data_socket, msg);
    return msg;
}

struct RaftMessage getBasicRaftMessage(){
    return createRaftMessage(1, true, 0, 0, 0, 0, 0, 0, 0, 3, "A\n\r");
}

struct RaftMessage getShutDownMessage(){
    return createRaftMessage(SHUTDOWN_MSG_TYPE, true, 1, 1, 1, 1, 1, 1, 1, 3, "A\n\r");
}

void clearBuffers() {
    memset(send_buffer, 0, sizeof(send_buffer));
    memset(recv_buffer, 0, sizeof(recv_buffer));
}

int getServerID(vector<sockaddr_in> addrs, int port) {
    for (int i = 0; i < addrs.size(); i++){
        if (addrs[i].sin_port == htons(port)){
            return i;
        }
    }
    cerr << "server address are misconfigured" << endl;
    exit(EXIT_FAILURE);
}



void assertRaftMessagesEqual(RaftMessage msg1, RaftMessage msg2){
    EXPECT_TRUE(msg1.type == msg2.type);
    EXPECT_TRUE(msg1.success == msg2.success);
    EXPECT_TRUE(msg1.senderID == msg2.senderID);
    EXPECT_TRUE(msg1.currentTerm == msg2.currentTerm);
    EXPECT_TRUE(msg1.prevLogTerm == msg2.prevLogTerm);
    EXPECT_TRUE(msg1.lastLogTerm == msg2.lastLogTerm);
    EXPECT_TRUE(msg1.prevLogIndex == msg2.prevLogIndex);
    EXPECT_TRUE(msg1.lastLogIndex == msg2.lastLogIndex);
    EXPECT_TRUE(msg1.commitIndex == msg2.commitIndex);
    EXPECT_TRUE(msg1.entriesLength == msg2.entriesLength);
    EXPECT_TRUE(msg1.entries == msg2.entries);
}

void assertCorrectAppendEntriesResponse(RaftMessage expected, RaftMessage actual){
    EXPECT_TRUE(expected.type == actual.type);
    EXPECT_TRUE(expected.success == actual.success);
    EXPECT_TRUE(expected.currentTerm == actual.currentTerm);
}



//TEST(ServerTest, ClientSendsRaftMessageToServer){
//    vector<pid_t> pids;
//    pid_t serverPID = startServer(1090);
//    pids.push_back(serverPID);
//    sleep(0.5);
//
//    vector<sockaddr_in> serverAddrs;
//    serverAddrs.push_back(createSocketAddress(1090));
//    vector<struct RaftMessage> messages;
//    messages.push_back(getBasicRaftMessage());
//    messages.push_back(getShutDownMessage());
//    pid_t clientPID = startClient(serverAddrs, messages);
//    pids.push_back(clientPID);
//
//    waitForAllProcesses(pids);
//}

TEST(ServerTest, ServerRespondsSuccessResponseForAppendEntriesWithIntialStateValues){
    int serverPort = 1090;
    int probePort = 1091;
    vector<sockaddr_in> serverAddrs;
    serverAddrs.push_back(createAddr(serverPort));
    serverAddrs.push_back(createAddr(probePort));
    vector<pid_t> pids;
    pid_t serverPID = startServer(serverPort, serverAddrs);
    pids.push_back(serverPID);
    sleep(0.5);

    probeListenSocket = createSocket();
    setSocketReuseOption(probeListenSocket);
    bindSocket(probeListenSocket, probePort);
    int ret = listen(probeListenSocket, 20);
    checkError(ret, "listen");
    clearBuffers();

    RaftMessage msg = createRaftMessage(1, true, getServerID(serverAddrs, probePort), 0, 0, 0, 0, 0, 0, 3, "A\n\r");
    RaftMessage shutdown = getShutDownMessage();

    probeSendSocket = createSocket();
    setSocketReuseOption(probeSendSocket);
    connectToServer(serverAddrs[0], probeSendSocket);



    sendRaftMessage(probeSendSocket, msg);
    close(probeSendSocket);
    probeSendSocket = createSocket();
    setSocketReuseOption(probeSendSocket);
    connectToServer(serverAddrs[0], probeSendSocket);
    sendRaftMessage(probeSendSocket, shutdown);

    close(probeSendSocket);

    int data_socket = acceptConnection(probeSendSocket);
    RaftMessage response = receiveRaftMessage(data_socket);
    close(data_socket);
    assertCorrectAppendEntriesResponse(createRaftMessage(2, true, getServerID(serverAddrs, serverPort), 0,0,0,0,0,0,0,""), response);
    waitForAllProcesses(pids);
}

