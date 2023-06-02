//
// Created by Owner on 6/1/2023.
//

#include <stdlib.h>
#include <string>
#include <cstdint>
#include "RaftMessage.h"

void serializeInt(const RaftMessage &msg, const std::vector<uint8_t> &buffer, uint8_t *valuePtr);

void serializeInt(RaftMessage &msg, std::vector<uint8_t> &buffer) {
    uint8_t* valuePtr = (uint8_t*)(&msg.type);
    buffer.insert(buffer.end(), valuePtr, valuePtr + sizeof(msg.type));
}

vector<uint8_t> serializeRaftMessage(RaftMessage &msg) {
    std::vector<uint8_t> buffer;

    uint8_t* valuePtr;
    serializeInt(msg, buffer);
    valuePtr = reinterpret_cast<uint8_t*>(&msg.success);
    buffer.insert(buffer.end(), valuePtr, valuePtr + sizeof(msg.success));
    valuePtr = reinterpret_cast<uint8_t*>(&msg.senderID);
    buffer.insert(buffer.end(), valuePtr, valuePtr + sizeof(msg.senderID));
    valuePtr = reinterpret_cast<uint8_t*>(&msg.currentTerm);
    buffer.insert(buffer.end(), valuePtr, valuePtr + sizeof(msg.currentTerm));
    valuePtr = reinterpret_cast<uint8_t*>(&msg.prevLogTerm);
    buffer.insert(buffer.end(), valuePtr, valuePtr + sizeof(msg.prevLogTerm));
    valuePtr = reinterpret_cast<uint8_t*>(&msg.lastLogTerm);
    buffer.insert(buffer.end(), valuePtr, valuePtr + sizeof(msg.lastLogTerm));
    valuePtr = reinterpret_cast<uint8_t*>(&msg.prevLogIndex);
    buffer.insert(buffer.end(), valuePtr, valuePtr + sizeof(msg.prevLogIndex));
    valuePtr = reinterpret_cast<uint8_t*>(&msg.lastLogIndex);
    buffer.insert(buffer.end(), valuePtr, valuePtr + sizeof(msg.lastLogIndex));
    valuePtr = reinterpret_cast<uint8_t*>(&msg.commitIndex);
    buffer.insert(buffer.end(), valuePtr, valuePtr + sizeof(msg.commitIndex));
    valuePtr = reinterpret_cast<uint8_t*>(&msg.entriesLength);
    buffer.insert(buffer.end(), valuePtr, valuePtr + sizeof(msg.entriesLength));

    buffer.insert(buffer.end(), (char*) &msg.entries, (char*) &msg.entries + sizeof(msg.entries));

    return buffer;

}

void deserializeRaftMessage(RaftMessage &msg, const uint8_t* buffer) {
    memcpy(&msg, buffer, sizeof(msg));
}

int getHeaderSize(RaftMessage msg){
    return sizeof msg.type + sizeof msg.success + sizeof msg.senderID + sizeof msg.currentTerm +
           sizeof msg.prevLogTerm + sizeof msg.lastLogTerm + sizeof msg.prevLogIndex + sizeof msg.lastLogIndex +
           sizeof msg.commitIndex + sizeof msg.entriesLength;
}

int getEntriesLengthOffset(struct RaftMessage msg){
    return sizeof msg.type + sizeof msg.success + sizeof msg.senderID + sizeof msg.currentTerm +
           sizeof msg.prevLogTerm + sizeof msg.lastLogTerm + sizeof msg.prevLogIndex + sizeof msg.lastLogIndex +
           sizeof msg.commitIndex;
}

RaftMessage createRaftMessage(int type, bool success, int senderId,
                                     int currentTerm, int prevLogTerm,
                                     int lastLogTerm, int prevLogIndex,
                                     int lastLogIndex, int commitIndex,
                                     int entriesLength,const string &entries) {
    RaftMessage msg;
    msg.type = type;
    msg.success = success;
    msg.senderID = senderId;
    msg.currentTerm = currentTerm;
    msg.prevLogTerm = prevLogTerm;
    msg.lastLogTerm = lastLogTerm;
    msg.prevLogIndex = prevLogIndex;
    msg.lastLogIndex = lastLogIndex;
    msg.commitIndex = commitIndex;
    msg.entriesLength = entriesLength;
    msg.entries = entries;
    return msg;
}

string raftMessageToString(RaftMessage msg){
    string result = "";
    result += "\nType: " + to_string(msg.type);
    result += "\nSuccess: " +  to_string(msg.success);
    result += "\nsenderID: " + to_string(msg.senderID);
    result += "\ncurrentTerm: " + to_string(msg.currentTerm);
    result += "\nprevLogTerm: " + to_string(msg.prevLogTerm);
    result += "\nlastLogTerm: " + to_string(msg.lastLogTerm);
    result += "\nprevLogIndex: " + to_string(msg.prevLogIndex);
    result += "\nlastLogIndex: " + to_string(msg.lastLogIndex);
    result += "\ncommitIndex: " + to_string(msg.commitIndex);
    result += "\nentriesLength: " + to_string(msg.entriesLength);
    result += "\nentries: " + msg.entries;
    return result;
}
