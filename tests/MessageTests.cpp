//
// Created by Owner on 5/27/2023.
//

#include <iostream>
#include <gtest/gtest.h>
#include "../RaftMessage.h"

TEST(MessageTest, MessageSerialization){
RaftMessage msg(8, true, 1, 2111222333, 2111222333, 2111222333,
                2111222333, 2111222333, 2111222333, 11, "Hello\n\rTest");
vector<uint8_t> data = msg.serialize();
RaftMessage msg2;
msg2.deserialize(data);
EXPECT_EQ(8, msg2.getType());
EXPECT_EQ(true, msg2.isSuccess());
EXPECT_EQ(1, msg2.getSenderId());
EXPECT_EQ(2111222333, msg2.getCurrentTerm());
EXPECT_EQ(2111222333, msg2.getPrevLogTerm());
EXPECT_EQ(2111222333, msg2.getLastLogTerm());
EXPECT_EQ(2111222333, msg2.getPrevLogIndex());
EXPECT_EQ(2111222333, msg2.getLastLogIndex());
EXPECT_EQ(2111222333, msg2.getCommitIndex());
EXPECT_EQ(11, msg2.getEntriesLength());
EXPECT_EQ("Hello\n\rTest", msg2.getEntries());
}
