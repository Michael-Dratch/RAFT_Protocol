//
// Created by Owner on 6/2/2023.
//

#ifndef RAFT_PROTOCOL_SERIALIZATION_H
#define RAFT_PROTOCOL_SERIALIZATION_H
#include <stdlib.h>
#include <cstdint>
#include "RaftMessage.h"

int deserializeInt(uint8_t*bufferPtr);
int deserializeBool(uint8_t*bufferPtr);
void serializeMessage(RaftMessage msg, uint8_t *packet);
void serializeEntries(RaftMessage msg, uint8_t *packet);
RaftMessage deserializeMessage(uint8_t* data);
#endif //RAFT_PROTOCOL_SERIALIZATION_H
