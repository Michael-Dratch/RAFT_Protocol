#include <iostream>
#include "RaftMessage.h"

using namespace std;

int main() {

    RaftMessage msg(8, true, 1, 2, 3, 2111222333,
            1, 2, 2111222333, 0, "");
    vector<uint8_t> data = msg.serialize();
    RaftMessage msg2;
    msg2.deserialize(data);
    cout << msg2.getType() << std::endl;
    cout << msg2.isSuccess() << std::endl;
    cout << msg2.getSenderId() << std::endl;
    cout << msg2.getCurrentTerm() << std::endl;
    cout << msg2.getPrevLogTerm() << std::endl;
    cout << msg2.getLastLogTerm() << std::endl;
    cout << msg2.getPrevLogIndex() << std::endl;
    cout << msg2.getLastLogIndex() << std::endl;
    cout << msg2.getCommitIndex() << std::endl;
    cout << msg2.getEntriesLength() << std::endl;
    return 0;
}
