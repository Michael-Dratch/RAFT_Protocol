
#include "Client.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <sys/un.h>



/*
 *  This function "starts the client".  It takes a header structure
 *  and a properly formed packet.  The packet is basically the
 *  header with a message at the end of it in the case of the
 *  command being CMD_PING_PONG.  The length in the header field
 *  indicates the total size to send to the server, thus it will
 *  be sizeof(cs472_proto_header_t) in the case that the cmd is
 *  CMD_COURSE_INFO, or it will be 
 *  sizeof(cs472_proto_header_t) + strlen(MESSAGE) + 1 in the case
 *  we are doing a ping to the server.  We add the extra 1 to send
 *  over the null terminator for the string
 */

void Client::start_client(uint8_t *packet){
    struct sockaddr_in addr;
    int data_socket;
    int ret;

    /* Create local socket. */

    data_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     */

    memset(&addr, 0, sizeof(struct sockaddr_in));

    /* Connect socket to socket address */

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT_NUM);

    ret = connect (data_socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_in));
    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }

    ret = send(data_socket, packet, strlen((const char *)packet), 0);
    if (ret == -1) {
        perror("header write error");
        exit(EXIT_FAILURE);
    }

    //NOW READ RESPONSES BACK - 2 READS, HEADER AND DATA
    ret = recv(data_socket, recv_buffer, sizeof(recv_buffer),0);
    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    printf("RECV FROM SERVER -> %s\n",recv_buffer);

    close(data_socket);

}
