#ifndef TRANSACTION_CLINET_H
#define TRANSACTION_CLINET_H

#include<client.h>

#include<connection_group.h>
#include<connection_thread_pool_manager.h>

// the only responsibility of the transaction_client is to encapsulate
// the connection_group and connection_thread_pool_manager for it
// the transaction_client is not responsible for allocating or releasing memory for the connection_group
// but the connection_thread_pool_manager will be completely managed by the transaction_client

typedef struct transaction_client transaction_client;
struct transaction_client
{
	connection_group* conn_group;
	connection_thread_pool_manager* manager;
};

// as you see in the get function, the transaction client, merely stores the pointer to the connection_group that you passed
// we do not create, manage or delete the memory of the connection group
transaction_client* get_transaction_client(connection_group* conn_group, unsigned long long int connection_count);

// the add_transaction_to function adds a transaction to be performed on a queue
// the transaction, can be performed by any of the connection, specified by tuple => (protocol, ip, port) of the connection_group
job* queue_transaction(transaction_client* tclient, void* (*transaction)(int fd, int* close_connection_requested, void* additional_params), void* additional_params);

/* ******************************************************************
anatomy of transaction function :
	void* (*transaction)(int fd, int* close_connection_requested, void* additional_params)
	parameters
		-> fd is the file discriptor of the open connection, on which you must talk to the server
		-> *close_connection_requested must be set to 1, if there was a connection failure and you want a ne connection to be formed
		-> additional_params, are the params, which we do not care about, they will be passed as is, they should be used by the client to create a request or query
	return
		-> you must return the pointer to the result formed after you have communicated with the server
		-> this same pointer will be returned back to you when you call get_result_for_transaction, but only after that transaction is completed
********************************************************************* */

// this function returns the result of the submitted transaction,
// it also give you pointer to the additional params submitted
void* get_result_for_transaction(job* job_p, void** additional_params);

// you must call shutdown_transaction_client before delete_transaction_client
// this method will close all the connections and wait till, all the threads complete
void shutdown_transaction_client(transaction_client* tclient);

// this function releases all the resources acquired by the transaction_client
// we do not free connection group here, because we did not allot memory for it
void delete_transaction_client(transaction_client* tclient);

#endif