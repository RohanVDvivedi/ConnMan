#include<client_set.h>

static stream* create_client_connection(client_set* cls)
{

}

static void destroy_client_connection(client_set* cls, stream* strm)
{

}

static int push_to_stream_queue(client_set* cls, stream* strm)
{
	if(is_full_queue(&(cls->active_clients_queue)))
		expand_queue(&(cls->active_clients_queue));

	return push_to_queue(&(cls->active_clients_queue), strm);
}

static stream* pop_from_stream_queue(client_set* cls)
{
	stream* strm = NULL;

	if(!is_empty_queue(&(cls->active_clients_queue)))
	{
		strm = (stream*) get_top_of_queue(&(cls->active_clients_queue));
		pop_from_queue(&(cls->active_clients_queue));
	}

	if(get_element_count_queue(&(cls->active_clients_queue)) * 3 >= get_capacity_queue(&(cls->active_clients_queue)))
		shrink_queue(&(cls->active_clients_queue));

	return strm;
}

client_set* new_client_set(const comm_address* server_addr_p, SSL_CTX* ssl_ctx, unsigned int max_clients)
{
	client_set* cls = malloc(sizeof(client_set));

	cls->server_addr = *server_addr_p;
	cls->ssl_ctx = ssl_ctx;

	if(max_clients == 0)
		max_clients = 1;
	cls->max_client_count = max_clients;

	cls->curr_client_count = 0;

	initialize_queue(&(cls->active_clients_queue), 0);

	pthread_mutex_init(&(cls->client_set_lock), NULL);
	
	pthread_cond_init(&(cls->all_clients_in_use_at_max_clients), NULL);
	pthread_cond_init(&(cls->client_count_reached_0_after_shutdown), NULL);

	cls->shutdown_called = 0;

	return cls;
}

unsigned int get_max_clients(client_set* cls)
{
	pthread_mutex_lock(&(cls->client_set_lock));
	unsigned int max_clients = cls->max_client_count;
	pthread_mutex_unlock(&(cls->client_set_lock));
	return max_clients;
}

int reset_max_clients(client_set* cls, unsigned int max_clients)
{
	if(max_clients == 0)
		max_clients = 1;

	int reset_max_clients_success = 0;

	pthread_mutex_lock(&(cls->client_set_lock));

	if(!cls->shutdown_called)
	{
		if(max_clients < cls->max_client_count)
		{
			// destroy additional clients from the queue
			unsigned int clients_to_destroy = cls->max_client_count - max_clients;

			// we can not destroy the clients that are still in use

			while(clients_to_destroy > 0 && !is_empty_queue(&(cls->active_clients_queue)))
			{
				stream* strm = pop_from_stream_queue(cls);
				destroy_client_connection(cls, strm);
				clients_to_destroy--;
			}
		}
		else if(max_clients > cls->max_client_count)
		{
			// wake only the additional number of threads that are waiting on all_clients_in_use_at_max_clients
			unsigned int additional_clients_addable = max_clients - cls->max_client_count;

			while(additional_clients_addable > 0)
			{
				pthread_cond_signal(&(cls->all_clients_in_use_at_max_clients));
				additional_clients_addable--;
			}
		}

		// reset value of max_client_count
		cls->max_client_count = max_clients;

		// mark return value as success
		reset_max_clients_success = 1;
	}

	pthread_mutex_unlock(&(cls->client_set_lock));

	return reset_max_clients_success;
}

stream* reserve_client(client_set* cls)
{
}

void return_client(client_set* cls, stream* strm)
{
	int client_to_be_destroyed = 0;

	pthread_mutex_lock(&(cls->client_set_lock));

	// these are the conditions -> to destroy the client
	if(strm->error || cls->shutdown_called || cls->curr_client_count > cls->max_client_count)
	{
		cls->curr_client_count--;

		// wake up a thread that wants shutdown and is waiting for all clients to be destroyed
		if(cls->shutdown_called && cls->curr_client_count == 0)
			pthread_cond_signal(&(cls->client_count_reached_0_after_shutdown));

		client_to_be_destroyed = 1;
	}

	pthread_mutex_unlock(&(cls->client_set_lock));

	// if the client connection was suppossed to be destroyed, then we do it here, outside the lock
	if(client_to_be_destroyed)
		destroy_client_connection(cls, strm);
}

void shutdown_and_delete_client_set(client_set* cls)
{
}