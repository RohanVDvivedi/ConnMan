#ifndef SERVER_H
#define SERVER_H

#include<connection_group.h>

#include<tcp_server_handler.h>
#include<udp_server_handler.h>

// here conn_fd, is the connection file discriptor, you can read and write to this discriptor, to communicate
// the client_addr, will hold the socket address of the client, we will this aswell to you
// returns the file discriptor to the tcp or udp socket, on which the server is listening to
int serve(connection_group* conn_grp_p, void (*handler)(int conn_fd), volatile int* listen_fd_p);

// stops the server that was started using the serve function
// it closes the file discriptor, hence the connection, here the parameter fd is the file discriptor of the listenning socket
int server_stop(int listen_fd);

#endif