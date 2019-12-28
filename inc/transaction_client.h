#ifndef TRANSACTION_CLINET_H
#define TRANSACTION_CLINET_H

#include<client.h>

#include<connection_group.h>
#include<connection_thread_pool_manager.h>

typedef struct transaction_client transaction_client;
struct transaction_client
{
	connection_group* conn_group;
	connection_thread_pool_manager* manager;
};

transaction_client* get_transaction_client(connection_group* conn_group, unsigned long long int connection_count);

// the add_transaction_to function adds a transaction to be performed on a queue
// the transaction, can be performed by any of the connection, specified by tuple => (protocol, ip, port)
void add_transaction_to(transaction_client* tclient, int (*transaction)(int fd, void* additional_params), void* additional_params);

// you must call shutdown_transaction_client before delete_transaction_client
// this method will close all the connections and wait till, all the threads complete
void shutdown_transaction_client(transaction_client* tclient);

// this function releases all the resources acquired by the transaction_client
void delete_transaction_client(transaction_client* tclient);

#endif