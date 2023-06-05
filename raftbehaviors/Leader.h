//
// Created by Owner on 6/5/2023.
//


#include "RaftBehavior.h"
#ifndef RAFT_PROTOCOL_LEADER_H
#define RAFT_PROTOCOL_LEADER_H

#define APPEND_ENTRIES_TYPE 1
#define APPEND_ENTRIES_RES_TYPE 2

class Leader: public RaftBehavior {
public:
    Leader(RaftHost *parentServer): RaftBehavior(parentServer){};
    RaftBehavior* handleAppendEntries(RaftMessage message) override;
    RaftBehavior* handleClientRequest(RaftMessage message) override;

private:

    RaftBehavior *getSameBehavior();

    RaftBehavior *getFollowerBehavior();

    vector<string> parseClientCommands(RaftMessage msg);

    string parseCommand(string &entries, int startingIndex);

    string createRaftEntries(vector<string> commands);

    void addCommandToEntriesString(const vector<string> &commands, string &entries, int i);

    void sendAppendEntriesToFollower(RaftMessage &appendEntries, int i);
};


#endif //RAFT_PROTOCOL_LEADER_H
