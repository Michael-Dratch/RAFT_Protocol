//
// Created by Owner on 6/4/2023.
//

#include "Follower.h"
#include "../server/Server.h"
#include "RaftBehavior.h"

//All functions related to handling append entries messages and sending correct responses from the follower state
RaftBehavior* Follower::handleAppendEntries(RaftMessage message) {
    cout << "HANDLING APPEND ENTRIES" << endl;
    updateCurrentTerm(message.currentTerm);
    if (doesAppendEntriesFail(message)){
        cout << "APPEND ENTRIES FAILS" << endl;
        sendAppendEntriesResponse(message, false);
        return getSameBehavior();
    }
    cout << "APPEND ENTRIES SUCCEEDS" << endl;
    addEntriesToLog(message);
    sendAppendEntriesResponse(message, true);
    printLog();
}

bool Follower::doesAppendEntriesFail(RaftMessage msg) {
    if (msg.currentTerm < server->currentTerm) {return true;}
    else if (msg.prevLogIndex == 0) {return false;}
    else if (server->log.size() < msg.prevLogIndex) {return true;}
    if (server->log.size() < 1) {return false;}
    else if( server->log.at(msg.prevLogIndex-1).term != msg.prevLogTerm) {return true;}
    return false;
}

void Follower::addEntriesToLog(RaftMessage msg) {
    string entries = msg.entries;
    int charIndex = 0;
    bool doneParsingEntries = false;
    if (msg.entriesLength <= 0){ doneParsingEntries = true;}
    while(!doneParsingEntries){
        Entry entry = parseEntry(entries, charIndex);
        server->log.push_back(entry);
        if (charIndex >= msg.entriesLength){
            doneParsingEntries = true;
        }
    }
}

Entry Follower::parseEntry(string &entries, int &charIndex) {
    int term = parseTerm(entries, charIndex);
    string value = parseValue(entries, charIndex);
    Entry entry;
    entry.term = term;
    entry.value = value;
    return entry;
}


int Follower::parseTerm(string &entries, int &i) {
    string termString = "";
    while (entries.at(i) != ' '){
        termString += entries.at(i);
        i++;
    }
    i++;
    int term;
    try {
        term = stoi(termString);
    } catch (const std::invalid_argument& e){
        cerr << "invalid argument exception: " << e.what() << endl;
        term = 0;
    }
    return term;
}

string Follower::parseValue(string &entries, int &i) {
    string value= "";
    while(!(entries.at(i) == '\n' && entries.at(i + 1) == '\r')){
        value += entries.at(i);
        i++;
    }
    i += 2;
    return value;
}

RaftBehavior *Follower::getSameBehavior() {
    RaftBehavior *sameBehavior;
    sameBehavior = this;
    return sameBehavior;
}

RaftBehavior *Follower::handleClientRequest(RaftMessage message) {
    cout << "redirect message to current leader" << endl;
    return getSameBehavior();
}
