//
// Created by Owner on 6/2/2023.
//

#include <cstdint>
#include <stdlib.h>
#include "Serialization.h"

int typeOffset = 0;
int successOffset = 4;
int senderIDOffset = 5;
int currentTermOffset = 9;
int prevLogTermOffset = 13;
int lastLogTermOffset = 17;
int prevLogIndexOffset = 21;
int lastLogIndexOffset = 25;
int commitIndexOffset = 29;
int entriesLengthOffset = 33;

int deserializeInt(uint8_t*bufferPtr) {
    uint8_t intData[4];
    for (int i = 0; i < 4; i++) {
        intData[i] = bufferPtr[i];
    }
    return (int) *intData;
}

int deserializeBool(uint8_t*bufferPtr) {
    uint8_t intData[1];
    intData[0] = bufferPtr[0];
    return (bool) *intData;
}

void serializeMessage(RaftMessage msg, uint8_t *packet){
    memcpy(packet + typeOffset, &msg.type, sizeof(msg.type));
    memcpy(packet + successOffset, &msg.success, sizeof(msg.success));
    memcpy(packet + senderIDOffset, &msg.senderID, sizeof(msg.senderID));
    memcpy(packet + currentTermOffset, &msg.currentTerm, sizeof(msg.currentTerm));
    memcpy(packet + prevLogTermOffset, &msg.prevLogTerm, sizeof(msg.prevLogTerm));
    memcpy(packet + lastLogTermOffset, &msg.lastLogTerm, sizeof(msg.lastLogTerm));
    memcpy(packet + prevLogIndexOffset, &msg.prevLogIndex, sizeof(msg.prevLogIndex));
    memcpy(packet + lastLogIndexOffset, &msg.lastLogIndex, sizeof(msg.lastLogIndex));
    memcpy(packet + commitIndexOffset, &msg.commitIndex, sizeof(msg.commitIndex));
    memcpy(packet + entriesLengthOffset, &msg.entriesLength, sizeof(msg.entriesLength));
}

void serializeEntries(RaftMessage msg, uint8_t *packet){
    const char* entries = msg.entries.c_str();
    memcpy(packet, &entries, sizeof(msg.entries));
}


        RaftMessage deserializeMessage(uint8_t* data){
RaftMessage msg;
    msg.type = deserializeInt(&data[typeOffset]);
    msg.success = deserializeBool(&data[successOffset]);
    msg.senderID = deserializeInt(&data[senderIDOffset]);
    msg.currentTerm = deserializeInt(&data[currentTermOffset]);
    msg.prevLogTerm = deserializeInt(&data[prevLogTermOffset]);
    msg.lastLogTerm = deserializeInt(&data[lastLogTermOffset]);
    msg.prevLogIndex = deserializeInt(&data[prevLogIndexOffset]);
    msg.lastLogIndex = deserializeInt(&data[lastLogIndexOffset]);
    msg.commitIndex = deserializeInt(&data[commitIndexOffset]);
    msg.entriesLength = deserializeInt(&data[entriesLengthOffset]);

    return msg;
}
