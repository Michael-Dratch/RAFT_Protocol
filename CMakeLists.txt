cmake_minimum_required(VERSION 3.25)
project(RAFT_Protocol)

set(CMAKE_CXX_STANDARD 17)

add_executable(RAFT_Protocol main.cpp RaftMessenger.cpp RaftMessenger.h)
