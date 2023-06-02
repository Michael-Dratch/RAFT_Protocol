/*
 * THE MOST BASIC SOCKET SERVER
 *
 */

#include <cstdint>

#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/un.h>
#include "../RaftMessage.h"


#define BUFF_SZ 512

#define PORT_NUM    1090

class Server{

public:
    void start_server();

    void process_requests(int listen_socket);

private:
    uint8_t send_buffer[BUFF_SZ];

    uint8_t recv_buffer[BUFF_SZ];

    void checkError(int ret, std::string message);

    void setSocketReuseOption(int listen_socket, int ret);

    void bindSocket(int listen_socket, int ret, sockaddr_in &addr);

    void clearBuffers();

    int acceptConnection(int listen_socket, int data_socket);
};