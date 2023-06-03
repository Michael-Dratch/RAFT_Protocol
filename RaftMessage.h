//
// Created by Owner on 5/26/2023.
//

#ifndef RAFT_PROTOCOL_RAFTMESSAGE_H
#define RAFT_PROTOCOL_RAFTMESSAGE_H

#include <string>
#include <array>
#include <charconv>
#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

struct RaftMessage {
    int type;
    bool success;
    int senderID;
    int currentTerm;
    int prevLogTerm;
    int lastLogTerm;
    int prevLogIndex;
    int lastLogIndex;
    int commitIndex;
    int entriesLength;
    string entries;
};


vector<uint8_t>  serializeRaftMessage(RaftMessage &data);

void deserializeRaftMessage(RaftMessage &msg, const uint8_t* buffer);

int getHeaderSize(RaftMessage msg);

int getEntriesLengthOffset(RaftMessage msg);

RaftMessage createRaftMessage(int type, bool success, int senderId,
                                        int currentTerm, int prevLogTerm,
                                        int lastLogTerm, int prevLogIndex,
                                        int lastLogIndex, int commitIndex,
                                        int entriesLength,const string &entries);

string raftMessageToString(struct RaftMessage msg);

void serializeInt(RaftMessage &msg, std::vector<uint8_t> &buffer);


#endif //RAFT_PROTOCOL_RAFTMESSAGE_H
