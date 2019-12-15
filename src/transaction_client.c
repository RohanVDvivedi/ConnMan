#include<transaction_client.h>

// this is a global hashmap, which will store a mapping from
// [ tcp or udp or any other protocol ] + [ip_address of the machine] + [port on the machine to talk to] -> queue_shared among the connections for the transactions 
hashmap* connection_to_transaction_queue = NULL;

void intialize_transaction_client()
{
	// todo
}

// converts "tcp://127.0.0.1:80" to a randon hash number
unsigned long long int connection_hash(dstring* connection_identifier)
{
	// todo
	return 0;
}

// returns things like "tcp://127.0.0.1:80"
void get_connection_identifier(dstring* identifer, sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT)
{
	// todo
}


// below are the implementation to the exposed functions, that can be used to actually create connections, add transactions to the connection queues and close the connections

void add_connections(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, int number_of_connections)
{
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