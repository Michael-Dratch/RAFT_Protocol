//
// Created by Owner on 5/26/2023.
//

#ifndef RAFT_PROTOCOL_RAFTMESSAGE_H
#define RAFT_PROTOCOL_RAFTMESSAGE_H

#include <string>
#include <array>
#include <charconv>
#include <vector>

using namespace std;

class RaftMessage {


private:
    short type;
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
    int headerSize;

    int getHeaderSize(){
        return sizeof type + sizeof success + sizeof senderID + sizeof currentTerm + sizeof prevLogTerm
        + sizeof lastLogTerm + sizeof prevLogIndex + sizeof lastLogIndex + sizeof commitIndex + sizeof entriesLength;
    }

public:
    RaftMessage(){headerSize = getHeaderSize();}

    RaftMessage(char type, bool success, int senderId, int currentTerm, int prevLogTerm, int lastLogTerm,
                int prevLogIndex, int lastLogIndex, int commitIndex, int entriesLength, const string &entries) :
                type(type),
                success(success),
                senderID(senderId),
                currentTerm(currentTerm),
                prevLogTerm(prevLogTerm),
                lastLogTerm(lastLogTerm),
                prevLogIndex(prevLogIndex),
                lastLogIndex(lastLogIndex),
                commitIndex(commitIndex),
                entriesLength(entriesLength),
                entries(entries) {
        printf("last log term constructor: %d", lastLogTerm);
        headerSize = getHeaderSize();}

    RaftMessage(char type, int commitIndex): type(type), commitIndex(commitIndex){
        headerSize = getHeaderSize();
    }

    vector<uint8_t> serialize(){
        vector<uint8_t> data;
        data.reserve(headerSize + sizeof entries);

        uint8_t serSenderID[4];
        uint8_t serCurrentTerm[4];
        uint8_t serPrevLogTerm[4];
        uint8_t serLastLogTerm[4];
        uint8_t serPrevLogIndex[4];
        uint8_t serLastLogIndex[4];
        uint8_t serCommitIndex[4];
        uint8_t serEntriesLength[4];


        serializeInt(senderID, serSenderID);
        serializeInt(currentTerm, serCurrentTerm);
        serializeInt(prevLogTerm, serPrevLogTerm);
        serializeInt(lastLogTerm, serLastLogTerm);
        serializeInt(prevLogIndex, serPrevLogIndex);
        serializeInt(lastLogIndex, serLastLogIndex);
        serializeInt(commitIndex, serCommitIndex);
        serializeInt(entriesLength, serEntriesLength);

        data.push_back(type);
        data.push_back(success);
        appendIntegerBytes(data, serSenderID);
        appendIntegerBytes(data, serCurrentTerm);
        appendIntegerBytes(data, serPrevLogTerm);
        appendIntegerBytes(data, serLastLogTerm);
        appendIntegerBytes(data, serPrevLogIndex);
        appendIntegerBytes(data, serLastLogIndex);
        appendIntegerBytes(data, serCommitIndex);
        appendIntegerBytes(data, serEntriesLength);
        printf("Finished serializing type: %d", data[0]);
        return data;
    }

    void serializeInt(int value, uint8_t * buffer) {
        for (int i = 0; i < sizeof(int); ++i) {
            buffer[i] = (char)((value >> (i * 8)) & 0xFF);
        }
    }

    void appendIntegerBytes(vector<uint8_t> &data, const uint8_t *value) const {
        data.push_back(value[0]);
        data.push_back(value[1]);
        data.push_back(value[2]);
        data.push_back(value[3]);
    }

    void deserialize(vector<uint8_t> data){
        type = data[0];
        success = data[1];
        senderID = deserializeInt(&data[2]);
        currentTerm = deserializeInt(&data[6]);
        prevLogTerm = deserializeInt(&data[10]);
        lastLogTerm = deserializeInt(&data[14]);
        prevLogIndex = deserializeInt(&data[18]);
        lastLogIndex = deserializeInt(&data[22]);
        commitIndex = deserializeInt(&data[26]);
        entriesLength = deserializeInt(&data[30]);
    }

    int deserializeInt(uint8_t * buffer) {
        int value = 0;
        for (int i = 0; i < sizeof(int); ++i) {
            value |= (static_cast<int>(buffer[i]) << (i * 8));
        }
        return value;
    }

    short getType() const {
        return type;
    }

    bool isSuccess() const {
        return success;
    }

    short getSenderId() const {
        return senderID;
    }

    short getCurrentTerm() const {
        return currentTerm;
    }

    short getPrevLogTerm() const {
        return prevLogTerm;
    }

    short getLastLogTerm() const {
        return lastLogTerm;
    }

    int getPrevLogIndex() const {
        return prevLogIndex;
    }

    int getLastLogIndex() const {
        return lastLogIndex;
    }

    int getCommitIndex() const {
        return commitIndex;
    }

    int getEntriesLength() const {
        return entriesLength;
    }

    const string &getEntries() const {
        return entries;
    }
};

#endif //RAFT_PROTOCOL_RAFTMESSAGE_H