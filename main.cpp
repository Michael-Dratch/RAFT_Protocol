#include <iostream>
#include  <unistd.h>
#include <string>
#include <gtest/gtest.h>

#include "RaftMessage.h"
#include "server/Server.h"
#include "client/Client.h"
using namespace std;

//int main() {
//    testing::InitGoogleTest();
//    return RUN_ALL_TESTS();
//}


int main(){
    pid_t serverPID = fork();
    if (serverPID == 0){
        cout << "starting server" << endl;
        Server s;
        s.start_server();
        exit(0);
    }

    sleep(1);
    pid_t clientPID = fork();

    if (clientPID == 0){
        cout << "starting client" << endl;
        Client c;
        unsigned char str= 'H';
        unsigned char* data = &str;
        c.start_client(data);
        exit(0);
    }

    waitpid(serverPID, NULL, 0);
    waitpid(clientPID, NULL, 0);
    cout << "Server test finished" << endl;
    return 0;
};


