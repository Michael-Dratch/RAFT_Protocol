//
// Created by Owner on 5/26/2023.
//

#ifndef RAFT_PROTOCOL_RAFTMESSENGER_H
#define RAFT_PROTOCOL_RAFTMESSENGER_H


class RaftMessenger {
public:
    void sendAppendEntries();

    void sendAppendEntriesResponse();

    void sendRequestVote();

    void sendRequestVoteResponse();
};


#endif //RAFT_PROTOCOL_RAFTMESSENGER_H
