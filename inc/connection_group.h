#ifndef CONNECTION_GROUP_H
#define CONNECTION_GROUP_H

#include<stdint.h>

#include<unistd.h>
#include<sys/types.h>
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

	union
	{
		struct sockaddr     ADDRESS;
		struct sockaddr_in  ADDRESS_ipv4;
		struct sockaddr_in6 ADDRESS_ipv6;
	};
};

connection_group get_connection_group_tcp_ipv4(char* SERVER_ADDRESS, uint16_t PORT);

connection_group get_connection_group_tcp_ipv6(char* SERVER_ADDRESS, uint16_t PORT);

connection_group get_connection_group_udp_ipv4(char* SERVER_ADDRESS, uint16_t PORT);

connection_group get_connection_group_udp_ipv6(char* SERVER_ADDRESS, uint16_t PORT);

#endif