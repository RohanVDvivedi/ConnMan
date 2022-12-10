#ifndef CLIENT_SET_H
#define CLIENT_SET_H

#include<queue.h>

#include<client.h>

#include<pthread.h>

// wait for this many microseconds until a connection is available to you
#define TIMEOUT_FOR_RESERVATION 100*1000

typedef struct client_set client_set;
struct client_set
{
	// comm_address of the server
	comm_address server_addr;

	// ssl_ctx for all client connections
	SSL_CTX* ssl_ctx;

	// the maximum client connection count that this client_set will hold
	unsigned int max_client_count;

	// this is the number of total active clients, (both reserved ones and the ones that are still in the queue)
	unsigned int curr_client_count;

	// a queue of all the active client connection stream, excluding the reserved ones
	queue active_clients_queue;

	// all the attributes of the client_set below the client_count_lock are thread-safed by this lock
	pthread_mutex_t client_set_lock;

	// a signal on this condition variable is called when a client_stream is returned or when max_client_count is increased
	pthread_cond_t all_clients_in_use_at_max_clients;

	// a signal on this condition variable is called only when shutdown_called is set and the curr_client_count reaches 0
	pthread_cond_t client_count_reached_0_after_shutdown;

	// it starts with a value of 0, and is set to 1 when a shutdown is called
	int shutdown_called;
};

// construct a new client_set connecting to the server_addr_p, each of it will be a ssl stream if ssl_ctx is not NULL 
client_set* new_client_set(const comm_address* server_addr_p, SSL_CTX* ssl_ctx, unsigned int max_clients);

// returns the max_clients count that is set on the client_set
unsigned int get_max_clients(client_set* cls);

// this call can be used to increment or decrement the max_client_count
// returns 0 on failure, it fails only if shutdown was called earlier
int reset_max_clients(client_set* cls, unsigned int max_clients);

// this function returns NULL, only when the shutdown is called for the client_set OR if you timed out
// pass timeout as 0, to perform an untimed wait
stream* reserve_client(client_set* cls, unsigned int timeout_in_secs);

// return a reserved client, this function will close and destroy the client stream if it has errored
void return_client(client_set* cls, stream* strm);

// waits for all the client streams to be returned
// you may delete the client_set only
void shutdown_and_delete_client_set(client_set* cls);

#endif