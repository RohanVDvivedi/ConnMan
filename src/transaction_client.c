#include<transaction_client.h>

typedef struct transaction_handler_params transaction_handler_params;
struct transaction_handler_params
{
	// this is the connection_group, the current transaction belongs to
	connection_group* belongs_to;

	// the additional_params, to be given to the transaction_function
	// the transaction is responsible to delete the additional params
	void* additional_params;

	// this is the transaction, that the transaction_handler has to execute
	// a transaction returns 0 for success, hence the connection will continue
	// return -1 (or any non zero), if the connection is corrupted, so the connection will be closed
	int (*transaction)(int fd, void* additional_params);
};

void* transaction_handler(transaction_handler_params* params)
{
	int fd = -1;

	connection_group* conn_grp_p = params->belongs_to;

	// find fd_p, the file_discriptor in the hashmap (thread_id_to_file_discriptor)
	fd = get_for_self(conn_grp_p->connection_mapping);

	// make connection, and insert entry, if the connection does not exist
	if(fd == -1)
	{
		write_lock(conn_grp_p->connection_mapping->thread_id_to_file_discriptor_lock);

		// find fd_p, the file_discriptor in the hashmap (thread_id_to_file_discriptor)
		fd = get_for_self(conn_grp_p->connection_mapping);

		// if not found make_connection, and insewrt the entry
		if(fd == -1)
		{
			// try to make a connection
			fd = make_connection(conn_grp_p->ADDRESS_FAMILY, conn_grp_p->TRANSMISSION_PROTOCOL_TYPE, conn_grp_p->SERVER_ADDRESS, conn_grp_p->PORT);

			// insert a new entry for thread_id_to_file_discriptor hashmap
			// we insert entry only if we were successfull in. making a connection
			if(fd != -1)
			{
				insert_self_UNSAFE(conn_grp_p->connection_mapping, fd);
			}
		}

		write_unlock(conn_grp_p->connection_mapping->thread_id_to_file_discriptor_lock);
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
		remove_self(conn_grp_p->connection_mapping);

		// close the connection
		close_connection(fd);
	}

	exit :;

	// delete params
	free(params);

	return NULL;
}

void add_transaction_to_connection_group(connection_group* conn_grp_p, int (*transaction)(int fd, void* additional_params), void* additional_params)
{
	transaction_handler_params* params = (transaction_handler_params*) malloc(sizeof(transaction_handler_params));
	params->belongs_to = conn_grp_p;
	params->additional_params = additional_params;
	params->transaction = transaction;
	submit(conn_grp_p->thread_pool, (void* (*)(void*))(transaction_handler), params);
}