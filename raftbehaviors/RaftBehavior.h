//
// Created by Owner on 6/4/2023.
//

#include "../server/Server.h"
#include "../RaftMessage.h"

#ifndef RAFT_PROTOCOL_RAFTBEHAVIOR_H
#define RAFT_PROTOCOL_RAFTBEHAVIOR_H


class RaftBehavior {
public:
    RaftBehavior(Server *parentServer){
        server = parentServer;
    }

    virtual void handleAppendEntries(RaftMessage message) = 0;
protected:
    Server *server;

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
