#ifndef TRANSACTION_CLINET_H
#define TRANSACTION_CLINET_H

#include<dstring.h>
#include<queue.h>
#include<hashmap.h>

#include<rwlock.h>

#include<executor.h>

#include<client.h>

#include<job.h>

// the connection_group is responsible for executing the transactions to be made on a uniquely identified connection type with multiple threads one for each connection
// For this purpose the connection_group takes as input the executor, the connection group is not responsible for handling the executor, it will work on whatever shared executor, you provide it with
typedef struct connection_group connection_group;
struct connection_group
{
	executor* transaction_executor;

	hashmap* thread_to_file_discriptor;
	rwlock* thread_to_file_discriptor_lock;

	// specifies if the protocol is tcp or ip
	int TRANSMISSION_PROTOCOL_TYPE;

	// ADDRESS_FAMILY specifies whether, it is an ipv4 or ipv6 connection
	sa_family_t ADDRESS_FAMILY; // AF_INET or AF_INET

	// ip address as a 32 bit integer, in host byte order
	uint32_t SERVER_ADDRESS;

	// the port on which the server is running
	uint16_t PORT;
};


// this function will take an executor service as input parameter, each of whose threads, are responsible for doing the transactions on their individual connections
// unless their connections break, the connection persists across the transactions
// the transaction_executor, that you pass in here as input parameter has to be solely maintained by you
connection_group* get_connection_group(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, executor* transaction_executor);

// the add_transaction_to function adds a transaction to be performed on a queue
// the transaction, can be performed by any of the connection, specified by tuple => (protocol, ip, port)
void add_transaction_to_connection_group(connection_group* conn_grp_p, void (*transaction)(int fd));

// this function will only delete the resources occupied by the connection_group
// this function must be called only after all the jobs in the transaction_executor have been completed
void delete_connection_group(connection_group* conn_grp_p);

#endif