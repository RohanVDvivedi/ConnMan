#ifndef CLIENT_SET_H
#define CLIENT_SET_H

#include<sync_queue.h>

#include<client.h>

// you may never exceed this client_count, it is equivalent to killing your or someone else's system
#define MAX_CLIENT_SET_CLIENT_COUNT 1024

// wait for this many microseconds until a connection is available to you
#define TIMEOUT_FOR_RESERVATION 100*1000

typedef struct client_set client_set;
struct client_set
{
	// comm_address of the server
	comm_address server_addr;

	// ssl_ctx for all client connections
	SSL_CTX* ssl_ctx;

	// a queue of all the active client connection stream, excluding the reserved ones
	sync_queue active_clients_queue;

	// all the attributes of the client_set below the client_count_lock are thread-safed by this lock
	pthread_mutex_t client_count_lock;

	// it starts with a value of 0, and is set to 1 when a shutdown is called
	int shutdown_called;

	// a signal on this condition variable is called only when shutdown_called is set and the curr_client_count reaches 0
	pthread_cond_t wait_for_all_clients_killed;

	// this is the number of total active clients, (both reserved ones and the ones that are still in the queue)
	unsigned int curr_client_count;

	// the maximum client connection count that this client_set will hold
	unsigned int max_client_count;
};

// construct a new client_set connecting to the server_addr_p, each of it will be a ssl stream if ssl_ctx is not NULL 
client_set* new_client_set(comm_address* server_addr_p, SSL_CTX* ssl_ctx, unsigned int max_clients);

// returns the max_clients count that is set on the client_set
unsigned int get_max_clients(client_set* cls);

// this call can be used to increment or decrement the max_client_count
// returns 0 on failure, it fails only if shutdown was called earlier
int reset_max_clients(client_set* cls, unsigned int max_clients);

// this function returns NULL, only when the shutdown is called for the client_set
stream* reserve_client(client_set* cls);

// return a reserved client, this function will close and destroy the client stream if it has errored
void return_client(client_set* cls, stream* strm);

// waits for all the client streams to be returned
void shutdown_client_set(client_set* cls);

// you may delete the client_set only after it has been shutdown
void delete_client_set(client_set* cls);

#endif