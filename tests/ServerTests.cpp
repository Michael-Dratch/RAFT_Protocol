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


void runClient() {
    cout << "starting client" << endl;
    Client c;
    unsigned char str= 'H';
    unsigned char* data = &str;
    c.start_client(data);
    exit(0);
}

void runServer() {
    cout << "starting server" << endl;
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






TEST(ServerTest, ServerTestStart){
    vector<pid_t> pids;
    pid_t serverPID = fork();
    if (serverPID == 0) {runServer();}
    pids.push_back(serverPID);

    sleep(0.5);

    pid_t clientPID = fork();
    if (clientPID == 0) { runClient(); }
    pids.push_back(clientPID);

    waitForAllProcesses(pids);
    cout << "Server test finished" << endl;
}