//
// Created by Owner on 5/28/2023.
//

#ifndef RAFT_PROTOCOL_CLIENT_H
#define RAFT_PROTOCOL_CLIENT_H
#include <stdint.h>

#define PORT_NUM    1090
#define EOF_CHAR_STR "\x05"   //CTRL+D is EOF in general ASCII 5

#define BUFF_SZ 512


class Client {
private:
    uint8_t send_buffer[BUFF_SZ] = {0};
    uint8_t recv_buffer[BUFF_SZ] = {0};
public:
    void start_client(uint8_t *packet_buff);
};


#endif //RAFT_PROTOCOL_CLIENT_H
