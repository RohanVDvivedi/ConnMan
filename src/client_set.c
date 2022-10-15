#include<client_set.h>

client_set* new_client_set(comm_address* server_addr_p, SSL_CTX* ssl_ctx, unsigned int max_clients)
{
	client_set* cls = malloc(sizeof(client_set));
	cls->server_addr = *server_addr_p;
	cls->ssl_ctx = ssl_ctx;
	initialize_sync_queue(&(cls->active_clients_queue), max_clients, MAX_CLIENT_SET_CLIENT_COUNT);
	pthread_mutex_init(&(cls->client_count_lock), NULL);
	cls->shutdown_called = 0;
	pthread_cond_init(&(cls->wait_for_all_clients_killed), NULL);
	cls->curr_client_count = 0;
	if(max_clients == 0)
		max_clients = 8;
	else if(max_clients > MAX_CLIENT_SET_CLIENT_COUNT)
		max_clients = MAX_CLIENT_SET_CLIENT_COUNT;
	cls->max_client_count = max_clients;
	return cls;
}

unsigned int get_max_clients(client_set* cls)
{
	pthread_mutex_lock(&(cls->client_count_lock));
	unsigned int max_clients = cls->max_client_count;
	pthread_mutex_unlock(&(cls->client_count_lock));
	return max_clients;
}

int reset_max_clients(client_set* cls, unsigned int max_clients)
{
	// fix for 0 and MAX_CLIENT_SET_CLIENT_COUNT max_clients
	if(max_clients == 0)
		max_clients = 8;
	else if(max_clients > MAX_CLIENT_SET_CLIENT_COUNT)
		max_clients = MAX_CLIENT_SET_CLIENT_COUNT;

	int reset_success = 0;
	pthread_mutex_lock(&(cls->client_count_lock));
	if(!cls->shutdown_called)
	{
		cls->max_client_count = max_clients;
		reset_success = 1;
	}
	pthread_mutex_unlock(&(cls->client_count_lock));
	return reset_success;
}

stream* reserve_client(client_set* cls)
{
	stream* strm = NULL;

	int shutdown_called = 0;
	int client_count_incremented = 0;

	pthread_mutex_lock(&(cls->client_count_lock));

	// cache the shutdown_called value, at this instance
	shutdown_called = cls->shutdown_called;

	// if the shutdown is not called, and the active clients queue is empty and there are less than max_client_count number of curr clients
	// then we are allowed to go ahead and make one for our selves
	if(!cls->shutdown_called && is_empty_sync_queue(&(cls->active_clients_queue)) && cls->curr_client_count < cls->max_client_count)
	{
		client_count_incremented = 1;
		cls->curr_client_count++;
	}
	pthread_mutex_unlock(&(cls->client_count_lock));

	// if the shutdown was called the last time we checked then we quit returning a NULL
	if(shutdown_called)
		return NULL;

	// if we were able to increment a the curr_client_count, then it is our responsibility to create a client and add it to the system
	if(client_count_incremented)
	{
		strm = malloc(sizeof(stream));
		if(make_connection_stream(strm, &(cls->server_addr), NULL, cls->ssl_ctx) == -1)
		{
			free(strm);
			strm = NULL;

			// since we failed to create a client, we decrement the curr_client_counter
			pthread_mutex_lock(&(cls->client_count_lock));
			cls->curr_client_count--;
			pthread_mutex_unlock(&(cls->client_count_lock));
		}
	}
	else
		// else we pop the client_connection stream from the queue, timing out in TIMEOUT_FOR_RESERVATION microseconds
		strm = (stream*) pop_sync_queue_blocking(&(cls->active_clients_queue), TIMEOUT_FOR_RESERVATION);

	return strm;
}

void return_client(client_set* cls, stream* strm)
{
	int client_stream_to_be_destroyed = 0;

	pthread_mutex_lock(&(cls->client_count_lock));
	if(strm->error || cls->shutdown_called || cls->curr_client_count > cls->max_client_count)
	{
		// we would be destroying a errored or an excess stream, so we need to decrement the curr_client_count 
		cls->curr_client_count--;
		client_stream_to_be_destroyed = 1;
	}
	pthread_mutex_unlock(&(cls->client_count_lock));

	if(client_stream_to_be_destroyed)
	{
		close_stream(strm);
		deinitialize_stream(strm);
		free(strm);
	}
	else // else we reinsert the stream to the active_clients_queue
	{
		push_sync_queue_blocking(&(cls->active_clients_queue), strm, TIMEOUT_FOR_RESERVATION);
	}
}

void shutdown_client_set(client_set* cls);

void delete_client_set(client_set* cls);