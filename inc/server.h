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

#define MAXIMUM_THREADS_IO_HANDLER 200											// maximum 200 threads for IO
#define NO_CONNECTION_THREAD_DESTROY_TIMEOUT_IN_MICRO_SECONDS 500 * 1000		// half a second

// here conn_fd, is the connection file discriptor, you can read and write to this discriptor, to communicate
// returns the failure response, in starting the server
int serve(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, unsigned long long int BACKLOG_QUEUE_SIZE, void (*connection_handler)(int conn_fd));


/*
	above function is very complicated with a log of parameters, so here below are a few smaller function for ther server
*/

#define DEFAULT_BACKLOG_QUEUE_SIZE 10

int serve_tcp_on_ipv4(uint16_t PORT, void (*connection_handler)(int conn_fd));

int serve_tcp_on_ipv6(uint16_t PORT, void (*connection_handler)(int conn_fd));

int serve_udp_on_ipv4(uint16_t PORT, void (*connection_handler)(int conn_fd));

int serve_udp_on_ipv6(uint16_t PORT, void (*connection_handler)(int conn_fd));

#endif