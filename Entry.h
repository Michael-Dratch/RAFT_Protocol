//
// Created by Owner on 6/3/2023.
//


#include <string>
#ifndef RAFT_PROTOCOL_ENTRY_H
#define RAFT_PROTOCOL_ENTRY_H

using namespace std;

struct Entry {
    int term;
    string value;
};



#endif //RAFT_PROTOCOL_ENTRY_H
