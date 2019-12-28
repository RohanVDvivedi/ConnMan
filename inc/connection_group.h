#ifndef CONNECTION_GROUP_H
#define CONNECTION_GROUP_H

#include<sys/socket.h>

#include<dstring.h>

#include<executor.h>

#include<connection_mapping.h>

// the connection_group is responsible for executing the transactions to be made on a uniquely identified connection type with multiple threads one for each connection
// For this purpose the connection_group takes as input the executor, the connection group is not responsible for handling the executor, it will work on whatever shared executor, you provide it with
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

	// this is the ececutor service the connection thread pool will be built on
	executor* thread_pool;

	// the threads of connection_group, have to remember which connection they were working on
	// this hashmap is shared among the threads to access, their own file discriptors
	connection_mapper* connection_mapping;
};

// returns things like "tcp://127.0.0.1:80"
void get_connection_group_identifier(dstring* identifer, connection_group* conn_grp_p);

// this function will take an executor service as input parameter, each of whose threads, are responsible for handling the connections individually
// unless their connections break, the connection persists across the transactions
// the transaction_executor, that you pass in here as input parameter has to be solely maintained by you
connection_group* get_connection_group(int TRANSMISSION_PROTOCOL_TYPE, sa_family_t ADDRESS_FAMILY, uint32_t SERVER_ADDRESS, uint16_t PORT, executor* thread_pool);

// this function will only delete the resources occupied by the connection_group
// this function must be called only after all the jobs in the transaction_executor have been completed
void delete_connection_group(connection_group* conn_grp_p);

// fast utils

connection_group* get_connection_group_tcp_ipv4(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd));

connection_group* get_connection_group_tcp_ipv6(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd));

connection_group* get_connection_group_udp_ipv4(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd));

connection_group* get_connection_group_udp_ipv6(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd));

#endif