#include<transaction_client.h>

// returns things like "tcp://127.0.0.1:80"
void get_connection_group_identifier(dstring* identifer, connection_group* conn_grp_p)
{
	make_dstring_empty(identifer);
	switch(conn_grp_p->TRANSMISSION_PROTOCOL_TYPE)
	{
		case SOCK_STREAM :
		{
			append_to_dstring(identifer, "tcp");
			break;
		}
		case SOCK_DGRAM :
		{
			append_to_dstring(identifer, "udp");
			break;
		}
	}

	append_to_dstring(identifer, "://");

	for(int i = 3; i >= 0; i--)
	{
		char temp[10] = "";
		uint32_t ip_part_dec = (conn_grp_p->SERVER_ADDRESS >> (i * 8)) & (0x000000ff);
		sprintf(temp, "%u", ip_part_dec);
		append_to_dstring(identifer, temp);

		if(i != 0)
		{
			append_to_dstring(identifer, ".");
		}
	}

	append_to_dstring(identifer, ":");

	char temp[10] = "";
	sprintf(temp, "%u", conn_grp_p->PORT);
	append_to_dstring(identifer, temp);
}

unsigned long long int thread_id_hash(const int* thread_id)
{
	return ((unsigned long long int)(*thread_id));
}

int compare_thread_ids(const int* a, const int* b)
{
	return ((*a) - (*b));
}

connection_group* get_connection_group(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, executor* transaction_executor)
{
	connection_group* conn_grp_p = (connection_group*) malloc(sizeof(connection_group));

	conn_grp_p->transaction_executor = transaction_executor;

	conn_grp_p->thread_id_to_file_discriptor = get_hashmap(10, (unsigned long long int (*)(const void* key))(thread_id_hash), (int (*)(const void* key1, const void* key2))(compare_thread_ids), ELEMENTS_AS_RED_BLACK_BST);
	conn_grp_p->thread_id_to_file_discriptor_lock = get_rwlock();

	conn_grp_p->ADDRESS_FAMILY = ADDRESS_FAMILY;
	conn_grp_p->TRANSMISSION_PROTOCOL_TYPE = TRANSMISSION_PROTOCOL_TYPE;
	conn_grp_p->SERVER_ADDRESS = SERVER_ADDRESS;
	conn_grp_p->PORT = PORT;

	return conn_grp_p;
}

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
	int thread_id = ((int)(pthread_self()));
	int* fd_p = NULL;

	connection_group* conn_grp_p = params->belongs_to;

	// find the connection, belonging to the current thread, using the given connection_group
	read_lock(conn_grp_p->thread_id_to_file_discriptor_lock);

	// find fd_p, the file_discriptor in the hashmap (thread_id_to_file_discriptor)
	fd_p = (int*) find_value_from_hash(conn_grp_p->thread_id_to_file_discriptor, &thread_id);

	read_unlock(conn_grp_p->thread_id_to_file_discriptor_lock);

	// make connection, and insert entry, if the connection does not exist
	if(fd_p == NULL)
	{
		write_lock(conn_grp_p->thread_id_to_file_discriptor_lock);

		// find fd_p, the file_discriptor in the hashmap (thread_id_to_file_discriptor)
		fd_p = (int*)find_value_from_hash(conn_grp_p->thread_id_to_file_discriptor, &thread_id);

		// if not found make_connection, and insewrt the entry
		if(fd_p == NULL)
		{
			// try to make a connection
			int fd = make_connection(conn_grp_p->ADDRESS_FAMILY, conn_grp_p->TRANSMISSION_PROTOCOL_TYPE, conn_grp_p->SERVER_ADDRESS, conn_grp_p->PORT);

			// insert a new entry for thread_id_to_file_discriptor hashmap
			// we insert entry only if we were successfull in. making a connection
			if(fd != -1)
			{
				fd_p = (int*)malloc(sizeof(int));
				(*fd_p) = fd;

				int* thread_id_key = (int*)malloc(sizeof(int));
				(*thread_id_key) = thread_id;
				insert_entry_in_hash(conn_grp_p->thread_id_to_file_discriptor, thread_id_key, fd_p);
			}
			else
			{
				fd_p = NULL;
			}
		}

		write_unlock(conn_grp_p->thread_id_to_file_discriptor_lock);
	}

	// the fd_p = NULL check here
	// is to confirm, if the connection, we tried to make was successfull
	if(fd_p == NULL)
	{
		printf("connection failed\n");
		goto exit;
	}

	int fd = (*fd_p);

	// execute the transaction
	int close_connection_requested = params->transaction(fd, params->additional_params);

	if(close_connection_requested)
	{
		write_lock(conn_grp_p->thread_id_to_file_discriptor_lock);

		// remove entry of fd_p, the file_discriptor in the hashmap (thread_id_to_file_discriptor)
		const void* rkey = NULL;
		const void* rvalue = NULL;
		int entry_deleted = delete_entry_from_hash(conn_grp_p->thread_id_to_file_discriptor, &thread_id, &rkey, &rvalue);
		if(entry_deleted)
		{
			if(rkey != NULL)
			{free((void*)rkey);}
			if(rvalue != NULL)
			{free((void*)rvalue);}
		}

		write_unlock(conn_grp_p->thread_id_to_file_discriptor_lock);

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
	submit(conn_grp_p->transaction_executor, (void* (*)(void*))(transaction_handler), params);
}

void delete_entry_operation(int* thread_id_p, int* file_discriptor_p, const void* ap)
{
	close_connection(*file_discriptor_p);
	free(thread_id_p);
	free(file_discriptor_p);
}

void delete_connection_group(connection_group* conn_grp_p)
{
	// delete all the elements of the thread_id_to_file_discriptor hashmap
	for_each_entry_in_hash(conn_grp_p->thread_id_to_file_discriptor, (void (*)(const void*, const void*, const void*))(delete_entry_operation), NULL);

	delete_hashmap(conn_grp_p->thread_id_to_file_discriptor);
	delete_rwlock(conn_grp_p->thread_id_to_file_discriptor_lock);
	free(conn_grp_p);
}