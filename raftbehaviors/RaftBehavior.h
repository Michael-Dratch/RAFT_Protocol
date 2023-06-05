//
// Created by Owner on 6/4/2023.
//
//
#ifndef RAFT_PROTOCOL_RAFTBEHAVIOR_H

#define RAFT_PROTOCOL_RAFTBEHAVIOR_H

#include "../RaftMessage.h"
#include "../RaftHost.h"

class RaftBehavior {
public:
    RaftBehavior(RaftHost *parentServer){
        server = parentServer;
    }

    virtual RaftBehavior* handleAppendEntries(RaftMessage message) = 0;
    virtual RaftBehavior* handleClientRequest(RaftMessage message) = 0;
protected:
    RaftHost *server;

    void sendAppendEntriesResponse(RaftMessage message, bool success);
    void connectToServer(sockaddr_in addr, int socket);
    int createSocket();
    void sendRaftMessage(int socket, RaftMessage msg);
    void sendPacket(int socket, uint8_t *packet, int packetSize);
    void updateCurrentTerm(int term);
    sockaddr_in getServerAddrFromID(int id);
    void printLog();
    void checkError(int ret, string message);
};


#endif //RAFT_PROTOCOL_RAFTBEHAVIOR_H
