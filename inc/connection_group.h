#ifndef CONNECTION_GROUP_H
#define CONNECTION_GROUP_H

#include<sys/socket.h>

#include<dstring.h>

#include<executor.h>

#include<connection_mapping.h>

// the connection_group is responsible to uniquely identify al connection group with multiple threads one for each connection,
typedef struct connection_group connection_group;
struct connection_group
{
	// specifies if the protocol is tcp or ip
	int TRANSMISSION_PROTOCOL_TYPE;

	// ADDRESS_FAMILY specifies whether, it is an ipv4 or ipv6 connection
	sa_family_t ADDRESS_FAMILY; // AF_INET or AF_INET6

	// ip address as a 32 bit integer, in host byte order
	uint32_t SERVER_ADDRESS;

	// the port on which the server is running
	uint16_t PORT;
};

// returns things like "tcp://127.0.0.1:80"
void get_connection_group_identifier(dstring* identifer, connection_group* conn_grp_p);

connection_group* get_connection_group(int TRANSMISSION_PROTOCOL_TYPE, sa_family_t ADDRESS_FAMILY, uint32_t SERVER_ADDRESS, uint16_t PORT);

void delete_connection_group(connection_group* conn_grp_p);

// fast utils

connection_group* get_connection_group_tcp_ipv4(uint32_t SERVER_ADDRESS, uint16_t PORT);

connection_group* get_connection_group_tcp_ipv6(uint32_t SERVER_ADDRESS, uint16_t PORT);

connection_group* get_connection_group_udp_ipv4(uint32_t SERVER_ADDRESS, uint16_t PORT);

connection_group* get_connection_group_udp_ipv6(uint32_t SERVER_ADDRESS, uint16_t PORT);

#endif