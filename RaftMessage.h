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

//};
//class RaftMessage {
//
//
//private:
//    int type;
//    bool success;
//    int senderID;
//    int currentTerm;
//    int prevLogTerm;
//    int lastLogTerm;
//    int prevLogIndex;
//    int lastLogIndex;
//    int commitIndex;
//    int entriesLength;
//    string entries;
//    int headerSize;
//
//public:
//
//    RaftMessage(){headerSize = getHeaderSize();}
//
//    RaftMessage(int type, bool success, int senderId, int currentTerm, int prevLogTerm, int lastLogTerm,
//                int prevLogIndex, int lastLogIndex, int commitIndex, int entriesLength, const string &entries) :
//                type(type),
//                success(success),
//                senderID(senderId),
//                currentTerm(currentTerm),
//                prevLogTerm(prevLogTerm),
//                lastLogTerm(lastLogTerm),
//                prevLogIndex(prevLogIndex),
//                lastLogIndex(lastLogIndex),
//                commitIndex(commitIndex),
//                entriesLength(entriesLength),
//                entries(entries) {
//        headerSize = getHeaderSize();}
//
//    uint8_t[] serialize(){
//        uint8_t data[headerSize + sizeof entries];
//        uint8_t serType[4];
//        uint8_t serSenderID[4];
//        uint8_t serCurrentTerm[4];
//        uint8_t serPrevLogTerm[4];
//        uint8_t serLastLogTerm[4];
//        uint8_t serPrevLogIndex[4];
//        uint8_t serLastLogIndex[4];
//        uint8_t serCommitIndex[4];
//        uint8_t serEntriesLength[4];
//
//        serializeInt(type, serType);
//        serializeInt(senderID, serSenderID);
//        serializeInt(currentTerm, serCurrentTerm);
//        serializeInt(prevLogTerm, serPrevLogTerm);
//        serializeInt(lastLogTerm, serLastLogTerm);
//        serializeInt(prevLogIndex, serPrevLogIndex);
//        serializeInt(lastLogIndex, serLastLogIndex);
//        serializeInt(commitIndex, serCommitIndex);
//        serializeInt(entriesLength, serEntriesLength);
//
//
//        appendIntegerBytes(data, serType);
//        data.push_back(success);
//        appendIntegerBytes(data, serSenderID);
//        appendIntegerBytes(data, serCurrentTerm);
//        appendIntegerBytes(data, serPrevLogTerm);
//        appendIntegerBytes(data, serLastLogTerm);
//        appendIntegerBytes(data, serPrevLogIndex);
//        appendIntegerBytes(data, serLastLogIndex);
//        appendIntegerBytes(data, serCommitIndex);
//        appendIntegerBytes(data, serEntriesLength);
//        appendString(data, entries.c_str());
//
//        return data;
//    }
//
//
//    static int getHeaderSize(){
//        return sizeof type + sizeof success + sizeof senderID + sizeof currentTerm + sizeof prevLogTerm
//               + sizeof lastLogTerm + sizeof prevLogIndex + sizeof lastLogIndex + sizeof commitIndex + sizeof entriesLength;
//    }
//
//    static int getEntriesLengthOffset(){
//        return sizeof type + sizeof success + sizeof senderID + sizeof currentTerm + sizeof prevLogTerm
//               + sizeof lastLogTerm + sizeof prevLogIndex + sizeof lastLogIndex + sizeof commitIndex;
//    }
//
//    static void serializeInt(int value, uint8_t * buffer) {
//            buffer[0] = (value >> 24) & 0xFF;
//            buffer[1] = (value >> 16) & 0xFF;
//            buffer[2] = (value >> 8) & 0xFF;
//            buffer[3] = value & 0xFF;
//    }
//
//    void appendIntegerBytes(vector<uint8_t> &data, const uint8_t *value) const {
//        data.push_back(value[0]);
//        data.push_back(value[1]);
//        data.push_back(value[2]);
//        data.push_back(value[3]);
//    }
//
//    void appendString(vector<uint8_t> &data, string string) {
//        for (int i = 0; i < string.size(); i++){
//            data.push_back(string[i]);
//        }
//    }
//
//    void deserialize(vector<uint8_t> data){
//        type = deserializeInt(&data[0]);
//        success = data[4];
//        senderID = deserializeInt(&data[5]);
//        currentTerm = deserializeInt(&data[9]);
//        prevLogTerm = deserializeInt(&data[13]);
//        lastLogTerm = deserializeInt(&data[17]);
//        prevLogIndex = deserializeInt(&data[21]);
//        lastLogIndex = deserializeInt(&data[25]);
//        commitIndex = deserializeInt(&data[29]);
//        entriesLength = deserializeInt(&data[33]);
//        entries = deserializeString(&data[37], entriesLength);
//    }
//
//
//
//    static string deserializeString(uint8_t * buffer, int stringLength) {
//        string result = "";
//        for (int i = 0; i < stringLength; i++){
//            result.push_back((char)buffer[i]);
//        }
//        return result;
//    }
//
//    static int deserializeInt(uint8_t * buffer) {
//        return (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
//    }
//
//    uint8_t getType() const {
//        return type;
//    }
//
//    bool isSuccess() const {
//        return success;
//    }
//
//    int getSenderId() const {
//        return senderID;
//    }
//
//    int getCurrentTerm() const {
//        return currentTerm;
//    }
//
//    int getPrevLogTerm() const {
//        return prevLogTerm;
//    }
//
//    int getLastLogTerm() const {
//        return lastLogTerm;
//    }
//
//    int getPrevLogIndex() const {
//        return prevLogIndex;
//    }
//
//    int getLastLogIndex() const {
//        return lastLogIndex;
//    }
//
//    int getCommitIndex() const {
//        return commitIndex;
//    }
//
//    int getEntriesLength() const {
//        return entriesLength;
//    }
//
//    const string &getEntries() const {
//        return entries;
//    }
//
//    string toString(){
//        string result = "";
//        result += "\nType: " + to_string(type);
//        result += "\nSuccess: " +  to_string(success);
//        result += "\nsenderID: " + to_string(senderID);
//        result += "\ncurrentTerm: " + to_string(currentTerm);
//        result += "\nprevLogTerm: " + to_string(prevLogTerm);
//        result += "\nlastLogTerm: " + to_string(lastLogTerm);
//        result += "\nprevLogIndex: " + to_string(prevLogIndex);
//        result += "\nlastLogIndex: " + to_string(lastLogIndex);
//        result += "\ncommitIndex: " + to_string(commitIndex);
//        result += "\nentriesLength: " + to_string(entriesLength);
//        result += "\nentries: " + entries;
//        return result;
//    }
//
//
//};

#endif //RAFT_PROTOCOL_RAFTMESSAGE_H
