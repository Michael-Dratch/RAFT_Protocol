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
using namespace std;


void waitForAllProcesses(vector<pid_t> pids);
void runServer();
void runClient();


void runClient(vector<sockaddr_in> serverAddrs, vector<RaftMessage> &messages) {
    Client c;
    c.start_client(serverAddrs, messages);
    exit(0);
}

void runServer() {
    Server s;
    s.start_server();
    exit(0);
};

void waitForAllProcesses(vector<pid_t> pids){
    int pidCount = pids.size();
    for (int i = 0; i < pidCount; i++){
        waitpid(pids[i], NULL, 0);
    }
}

pid_t startServer(){
    pid_t serverPID = fork();
    if (serverPID == 0) {runServer();}
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

struct RaftMessage getBasicRaftMessage(){
    return createRaftMessage(1, true, 1, 1, 1, 1, 1, 1, 1, 3, "A\n\r");
}

//void assertRaftMessagesEqual(RaftMessage msg1, RaftMessage msg2){
//    EXPECT_TRUE(msg1.getType() == msg2.getType());
//    EXPECT_TRUE(msg1.isSuccess() == msg2.isSuccess());
//    EXPECT_TRUE(msg1.getSenderId() == msg2.getSenderId());
//    EXPECT_TRUE(msg1.getCurrentTerm() == msg2.getCurrentTerm());
//    EXPECT_TRUE(msg1.getPrevLogTerm() == msg2.getPrevLogTerm());
//    EXPECT_TRUE(msg1.getLastLogTerm() == msg2.getLastLogTerm());
//    EXPECT_TRUE(msg1.getPrevLogIndex() == msg2.getPrevLogIndex());
//    EXPECT_TRUE(msg1.getLastLogIndex() == msg2.getLastLogIndex());
//    EXPECT_TRUE(msg1.getCommitIndex() == msg2.getCommitIndex());
//    EXPECT_TRUE(msg1.getEntriesLength() == msg2.getEntriesLength());
//    EXPECT_TRUE(msg1.getEntries() == msg2.getEntries());
//}



//TEST(ServerTest, ServerTestStart){
//    vector<pid_t> pids;
//    pid_t serverPID = startServer();
//    pids.push_back(serverPID);
//
//    sleep(0.5);
//
//    pid_t clientPID = startClient();
//    pids.push_back(clientPID);
//
//    waitForAllProcesses(pids);
//}

TEST(ServerTest, ClientSendsRaftMessageToServer){
    vector<pid_t> pids;
    pid_t serverPID = startServer();
    pids.push_back(serverPID);
    cout << "TEST: SERVER STARTED" << endl;
    sleep(0.5);

    vector<sockaddr_in> serverAddrs;
    serverAddrs.push_back(createSocketAddress(1090));
    vector<struct RaftMessage> messages;
    messages.push_back(getBasicRaftMessage());
    cout << "TEST: STARTING CLIENT" << endl;
    pid_t clientPID = startClient(serverAddrs, messages);
    pids.push_back(clientPID);

    waitForAllProcesses(pids);
    cout << "TEST FINISHED"<< endl;
}

