//
// Created by Owner on 5/26/2023.
//


/*
 * THE MOST BASIC SOCKET SERVER
 *
 */

#include <cstdint>

#include <sys/socket.h>
#include <cstdlib>
#include <iostream>
#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/un.h>
#include "Server.h"

using namespace std;

void Server::process_requests(int listen_socket){
        int data_socket;
        int ret;

        //again, not the best approach, need ctrl-c to exit

        //Do some cleaning
        memset(send_buffer,0,sizeof(send_buffer));
        memset(recv_buffer,0,sizeof(recv_buffer));

        //Establish a connection
        data_socket = accept(listen_socket, NULL, NULL);
        if (data_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("\t RECEIVED REQ...\n");

        /* Wait for next data packet. */
        ret = recv(data_socket, recv_buffer, sizeof(recv_buffer),0);
        if (ret == -1) {
            perror("read error");
            exit(EXIT_FAILURE);
        }

        int buff_len = sprintf((char *)send_buffer, "THANK YOU -> %s", recv_buffer);

        //now string out buffer has the length
        send (data_socket, send_buffer, buff_len, 0);
        close(data_socket);
        cout << "Sent Back Message" << endl;

    }

/*
 *  This function starts the server, basically creating the socket
 *  it will listen on INADDR_ANY which is basically all local
 *  interfaces, eg., 0.0.0.0
 */
void Server::start_server(){
        int listen_socket;
        int ret;

        struct sockaddr_in addr;

        /* Create local socket. */
        listen_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (listen_socket == -1) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        int reuse = 1;
        if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
            std::cerr << "Failed to set the SO_REUSEADDR option." << std::endl;
            exit(EXIT_FAILURE);
        }


        /* Bind socket to socket name. */
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        addr.sin_port = htons(PORT_NUM);

        ret = bind(listen_socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_in));
        if (ret == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        /*
         * Prepare for accepting connections. The backlog size is set
         * to 20. So while one request is being processed other requests
         * can be waiting.
         */
        ret = listen(listen_socket, 20);
        if (ret == -1) {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        //Now process requests, this will never return so its bad coding
        //but ok for purposes of demo
        process_requests(listen_socket);

        close(listen_socket);
    }





