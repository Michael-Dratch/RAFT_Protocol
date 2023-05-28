cmake_minimum_required(VERSION 3.21)
project(RAFT_Protocol)

set(CMAKE_CXX_STANDARD 17)

add_executable(RAFT_Protocol main.cpp server/Server.cpp server/Server.h RaftMessage.cpp RaftMessage.h tests/MessageTests.cpp tests/ServerTests.cpp client/Client.cpp client/Client.h)
target_link_libraries(RAFT_Protocol gtest gtest_main)
