#include<transaction_client.h>

typedef struct transaction_handler_params transaction_handler_params;
struct transaction_handler_params
{
	// the additional_params, to be given to the transaction_function
	// the transaction is responsible to delete the additional params
	void* additional_params;

	// this is the transaction, that the transaction_handler has to execute
	// a transaction returns 0 for success, hence the connection will continue
	// return -1 (or any non zero), if the connection is corrupted, so the connection will be closed
	int (*transaction)(int fd, void* additional_params);

	// this is the transaction_client, that this transaction belongs to
	// tclient->manager->connection_mapping where you need to explicitly specify, on what connection file discriptor you are working with
	// tclient->connection_group, which you will have to use to open new connection
	transaction_client* tclient;
};

void* transaction_handler(transaction_handler_params* params);

transaction_client* get_transaction_client(connection_group* conn_group, unsigned long long int connection_count)
{
	transaction_client* tclient = (transaction_client*) malloc(sizeof(transaction_client));
	tclient->conn_group = conn_group;
	tclient->manager = get_fixed_connection_thread_pool_manager(connection_count, (void* (*)(void*))(transaction_handler));
	return tclient;
}

void* transaction_handler(transaction_handler_params* params)
{
	int fd = -1;

	// find fd_p, that can be used by the current thread
	fd = get_file_discriptor_for_current_thread(params->tclient->manager);

	// if not found make_connection, and insert the entry
	if(fd == -1)
	{
		// try to make a connection
		fd = make_connection(params->tclient->conn_group);

		// insert a new entry for thread_id_to_file_discriptor hashmap
		// we insert entry only if we were successfull in. making a connection
		if(fd != -1)
		{
			insert_mapping_with_current_thread(params->tclient->manager, fd);
		}
	}

	// the fd_p = NULL check here
	// is to confirm, if the connection, we tried to make was successfull
	if(fd == -1)
	{
		printf("making connection failed\n");
		goto exit;
	}

	// execute the transaction
	int close_connection_requested = params->transaction(fd, params->additional_params);

	if(close_connection_requested)
	{
		remove_mapping_for_current_thread(params->tclient->manager);

		// close the connection
		close_connection(fd);
	}

	exit :;

	// delete params
	free(params);

	return NULL;
}

void queue_transaction(transaction_client* tclient, int (*transaction)(int fd, void* additional_params), void* additional_params)
{
	transaction_handler_params* params = (transaction_handler_params*) malloc(sizeof(transaction_handler_params));
	params->tclient = tclient;
	params->additional_params = additional_params;
	params->transaction = transaction;
	submit_job_parameters(tclient->manager, params);
}

void shutdown_transaction_client(transaction_client* tclient)
{
	close_all_connections_and_wait_for_shutdown(tclient->manager, 1);
}

void delete_transaction_client(transaction_client* tclient)
{
	delete_connection_thread_pool_manager(tclient->manager);
	free(tclient);
}