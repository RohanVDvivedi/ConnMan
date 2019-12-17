#ifndef CLIENT_H
#define CLIENT_H

// temporary code below

#include<stdio.h> /* printf, sprintf */
#include<stdlib.h> /* exit */
#include<unistd.h> /* read, write, close */
#include<string.h> /* memcpy, memset */
#include<sys/socket.h> /* socket, connect */
#include<netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include<netdb.h> /* struct hostent, gethostbyname */

int connect_to(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, void (*handler)(int fd));

// the below are the sub functions used to build the connect_to functions

// returns file-discriptor to the socket, through which client connection has been made
int make_connection(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT);

// returns 0, if the connection was closed successfully
int close_connection(int fd);

//

int connect_to_tcp_on_ipv4(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd));

int connect_to_tcp_on_ipv6(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd));

int connect_to_udp_on_ipv4(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd));

int connect_to_udp_on_ipv6(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd));

#endif