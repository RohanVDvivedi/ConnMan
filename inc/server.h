#ifndef SERVER_H
#define SERVER_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

#include<executor.h>

// here conn_fd, is the connection file discriptor, you can read and write to this discriptor, to communicate
// returns the failure response, in starting the server
int serve(int DOMAIN, int TRANSMISSION_PROTOCOL_TYPE, sa_family_t ADDRESS_FAMILY, uint32_t SERVER_ADDRESS, unsigned long long int PORT, unsigned long long int backlog_queue_size, void (*connection_handler)(int conn_fd));

#endif