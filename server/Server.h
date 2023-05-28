/*
 * THE MOST BASIC SOCKET SERVER
 *
 */

#include <cstdint>

#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/un.h>

#define BUFF_SZ 512

#define PORT_NUM    1090

class Server{
private:
    uint8_t send_buffer[BUFF_SZ];
    uint8_t recv_buffer[BUFF_SZ];

public:
void process_requests(int listen_socket);
void start_server();
};