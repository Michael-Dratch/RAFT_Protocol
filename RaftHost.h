//
// Created by Owner on 6/4/2023.
//
#include <netinet/in.h>
#include <vector>
#include "Entry.h"
#include "./raftbehaviors/RaftBehavior.h"

#ifndef RAFT_PROTOCOL_RAFTHOST_H
#define RAFT_PROTOCOL_RAFTHOST_H


class RaftHost {
public:
    vector<Entry> log;

    int serverID;

    int currentTerm;

    vector<sockaddr_in> serverAddresses;

    int commitIndex;

    int lastApplied;
};


#endif //RAFT_PROTOCOL_RAFTHOST_H
