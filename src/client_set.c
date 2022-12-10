#include<client_set.h>

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

			// mark return value as success
			reset_max_clients_success = 1;
		}
		else if(max_clients > cls->max_client_count)
		{

			// mark return value as success
			reset_max_clients_success = 1;
		}
	}

	pthread_mutex_unlock(&(cls->client_set_lock));

	return reset_max_clients_success;
}

stream* reserve_client(client_set* cls)
{
}

void return_client(client_set* cls, stream* strm)
{
}

void shutdown_and_delete_client_set(client_set* cls)
{
}