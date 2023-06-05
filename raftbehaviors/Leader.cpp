//
// Created by Owner on 6/5/2023.
//
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>

#include "Leader.h"
#include "Follower.h"
#include "../RaftMessage.h"

// leader only checks append entries messages to ensure server term is not out of date
RaftBehavior* Leader::handleAppendEntries(RaftMessage message) {
    if (message.currentTerm > server->currentTerm){return getFollowerBehavior();}
    else {return getSameBehavior();}
}

RaftBehavior *Leader::getFollowerBehavior() {
    RaftBehavior* behavior;
    Follower follower(server);
    behavior = &follower;
    return behavior;
}

RaftBehavior *Leader::getSameBehavior() {
    RaftBehavior* behavior;
    behavior = this;
    return behavior;
}

// Functions for handling client update requests and sending correct append entries messages from Leader state
RaftBehavior *Leader::handleClientRequest(RaftMessage message) {
    vector<string> commands = parseClientCommands(message);
    string raftEntries = createRaftEntries(commands);
    RaftMessage appendEntries = createRaftMessage(APPEND_ENTRIES_TYPE,
                                                  true,
                                                  server->serverID,
                                                  server->currentTerm,
                                                  0, 0, 0, 0,
                                                  server->commitIndex,
                                                  raftEntries.size(),
                                                  raftEntries);
    for (int i = 0; i < server->serverAddresses.size(); i++) {
        if (i == server->serverID) {continue;}
        sendAppendEntriesToFollower(appendEntries, i);
    }
    cout <<"done handling client request" << endl;
    return getSameBehavior();
}

void Leader::sendAppendEntriesToFollower(RaftMessage &appendEntries, int i) {
    int socket = createSocket();
    connectToServer(server->serverAddresses[i], socket);
    sendRaftMessage(socket, appendEntries);
    close(socket);
}

vector<string> Leader::parseClientCommands(RaftMessage msg) {
    vector<string> commands;
    int nextIndex  = 0;
    bool doneParsingEntries = false;
    if (msg.entriesLength <= 0){
        doneParsingEntries = true;
    }
    while(!doneParsingEntries){
        string command = parseCommand(msg.entries, nextIndex);
        nextIndex = command.size()+2;
        commands.push_back(command);
        if (nextIndex >= msg.entriesLength){
            doneParsingEntries = true;
        }
    }
    return commands;
}

string Leader::parseCommand(string &entries, int startingIndex) {
    string command = "";
    while (!(entries.at(startingIndex) == '\n' && entries.at(startingIndex+1) == '\r')){
        command += entries.at(startingIndex);
        startingIndex++;
    }
    return command;
}

string Leader::createRaftEntries(vector<string> commands){
    string entries = "";
    for (int i = 0; i < commands.size(); i++){
        addCommandToEntriesString(commands, entries, i);
    }
    return entries;
}

void Leader::addCommandToEntriesString(const vector<string> &commands, string &entries, int i) {
    entries += to_string(server->currentTerm);
    entries += ' ';
    entries += commands[i];
    entries += "\n\r";
}
