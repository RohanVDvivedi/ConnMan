#include<transaction_client.h>

// converts "tcp://127.0.0.1:80" to a randon hash number
unsigned long long int connection_hash(dstring* connection_identifier)
{
	return connection_identifier->bytes_occupied;
}

// returns things like "tcp://127.0.0.1:80"
void get_connection_identifier(dstring* identifer, sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT)
{
	make_dstring_empty(identifer);
	switch(TRANSMISSION_PROTOCOL_TYPE)
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
		uint32_t ip_part_dec = (SERVER_ADDRESS >> (i * 8)) & (0x000000ff);
		sprintf(temp, "%u", ip_part_dec);
		append_to_dstring(identifer, temp);

		if(i != 0)
		{
			append_to_dstring(identifer, ".");
		}
	}

	append_to_dstring(identifer, ":");

	char temp[10] = "";
	sprintf(temp, "%u", PORT);
	append_to_dstring(identifer, temp);
}


// this is a global hashmap, which will store a mapping from
// [ tcp or udp or any other protocol ] + [ip_address of the machine] + [port on the machine to talk to] -> queue_shared among the connections for the transactions 
// effectively dstring* vs queue* (queue of dstring*)
hashmap* connection_to_transaction_queue = NULL;
rwlock* connection_access_protector = NULL;

typedef struct protected_transaction_queue protected_transaction_queue;
struct protected_transaction_queue
{
	queue* transaction_queue;
	rwlock* transaction_queue_lock;
};

protected_transaction_queue* get_protected_transaction_queue()
{
	// todo
	return NULL;
}

void queue_transaction(protected_transaction_queue* pt_queue, dstring* transaction)
{

}

void delete_protected_transaction_queue(protected_transaction_queue* pt_queue)
{
	// todo
}




// below are the implementation to the exposed functions, that can be used to actually create connections, add transactions to the connection queues and close the connections

void intialize_transaction_client()
{
	if(connection_to_transaction_queue == NULL)
	{
		connection_to_transaction_queue = get_hashmap(10, (unsigned long long int (*)(const void*))(connection_hash), (int (*)(const void* key1, const void* key2))(compare_dstring), ELEMENTS_AS_RED_BLACK_BST);
		connection_access_protector = get_rwlock();
	}
}

void add_connections(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, int number_of_connections)
{
	dstring* url_key = get_dstring("", 10);
	get_connection_identifier(url_key, ADDRESS_FAMILY, TRANSMISSION_PROTOCOL_TYPE, SERVER_ADDRESS, PORT);

	// todo
}

void add_transaction_to(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, void (*transaction)(int fd))
{
	// todo
}

void close_all_connections_to(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT)
{
	// todo
}

void destroy_transaction_client()
{
	// todo
}