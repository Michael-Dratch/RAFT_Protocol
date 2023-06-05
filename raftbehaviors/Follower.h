//
// Created by Owner on 6/4/2023.
//
#include <string>
#include "../RaftMessage.h"
#include "RaftBehavior.h"
#include "../RaftHost.h"
#include "../Entry.h"

#ifndef RAFT_PROTOCOL_FOLLOWER_H
#define RAFT_PROTOCOL_FOLLOWER_H

class Follower: public RaftBehavior {
public:
    Follower(RaftHost *parentServer): RaftBehavior(parentServer){};
    RaftBehavior* handleAppendEntries(RaftMessage message) override;
    RaftBehavior* handleClientRequest(RaftMessage message) override;

private:
    bool doesAppendEntriesFail(RaftMessage message);
    void addEntriesToLog(RaftMessage msg);
    int parseTerm(string &entries, int &i);
    string parseValue(string &entries, int &i);
    Entry parseEntry(string &entries, int &charIndex);
    RaftBehavior *getSameBehavior();
};


#endif
