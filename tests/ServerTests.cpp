//
// Created by Owner on 5/28/2023.
//

#include <iostream>
#include <gtest/gtest.h>
#include <string>
#include "../server/Server.h"
#include "../client/Client.h"
using namespace std;

TEST(ServerTest, ServerTestStart){
    cout << "starting server" << endl;
    Server s;
    s.start_server();
    cout << "starting client" << endl;
    Client c;
    unsigned char str= 'H';
    unsigned char* data = &str;
    c.start_client(data);
    cout << "Server test finished" << endl;


}