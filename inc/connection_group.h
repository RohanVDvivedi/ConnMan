#ifndef CONNECTION_GROUP_H
#define CONNECTION_GROUP_H

#include<stdint.h>

#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<dstring.h>

// the connection_group is responsible to uniquely identify al connection group with multiple threads one for each connection,
typedef struct connection_group connection_group;
struct connection_group
{
	// specifies if the protocol is tcp or ip
	int PROTOCOL;

	// we use sockaddr_in6, this is required so we can accomodate both ipv4 and ipv6
	struct sockaddr_in ADDRESS;
};

// returns things like "tcp://127.0.0.1:80"
void get_connection_group_identifier(dstring* identifer, connection_group* conn_grp_p);

// fast utils

connection_group get_connection_group_tcp_ipv4(char* SERVER_ADDRESS, uint16_t PORT);

connection_group get_connection_group_tcp_ipv6(char* SERVER_ADDRESS, uint16_t PORT);

connection_group get_connection_group_udp_ipv4(char* SERVER_ADDRESS, uint16_t PORT);

connection_group get_connection_group_udp_ipv6(char* SERVER_ADDRESS, uint16_t PORT);

#endif