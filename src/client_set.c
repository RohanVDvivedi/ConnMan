#include<client_set.h>

static stream* create_client_connection(client_set* cls)
{
	stream* strm = malloc(sizeof(stream));
	if(make_connection_stream(strm, &(cls->server_addr), NULL, cls->ssl_ctx) == -1)
	{
		free(strm);
		strm = NULL;
	}
	return strm;
}

static void destroy_client_connection(client_set* cls, stream* strm)
{
	close_stream(strm);
	deinitialize_stream(strm);
	free(strm);
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

				// decrement the curr_client_count
				cls->curr_client_count--;

				// here since shutdown has not been called, we do not need to wake up thread that called shutdown
				// we do not need to wake up any one waiting on the condition variable client_count_reached_0_after_shutdown

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

stream* reserve_client(client_set* cls, unsigned int timeout_in_secs)
{
	stream* strm = NULL;

	int was_shutdown_called = 0;
	int create_client_allowed = 0;

	pthread_mutex_lock(&(cls->client_set_lock));
	
	// wait until you are not allowed to create a client and the active client queue is empty
	while(!cls->shutdown_called && cls->curr_client_count >= cls->max_client_count && is_empty_queue(&(cls->active_clients_queue)))
	{
		// if a valid timeout is passed, then perform a timed wait
		if(timeout_in_secs > 0)
		{
			struct timespec current_time;
			clock_gettime(CLOCK_REALTIME, &current_time);

			struct timespec wait_till = current_time;
			wait_till.tv_sec += timeout_in_secs;

			int timed_out = pthread_cond_timedwait(&(cls->all_clients_in_use_at_max_clients), &(cls->client_set_lock), &wait_till);
			if(timed_out)
				break;
		}
		// else perform a conventional untimed wait
		else
			pthread_cond_wait(&(cls->all_clients_in_use_at_max_clients), &(cls->client_set_lock));
	}

	// upon exit from the loop, we are in any of the following 3 states
	// 1. shutdown is called
	// 2. a client in the active_client queue
	// 3. we are allowed to create a new client connection
	// 4. none of the above because we timed out and now we need to quit
	if(cls->shutdown_called)
		was_shutdown_called = 1;
	else if(is_empty_queue(&(cls->active_clients_queue)))
		strm = pop_from_stream_queue(cls);
	else if(cls->curr_client_count < cls->max_client_count)
	{
		cls->curr_client_count++;
		create_client_allowed = 1;
	}

	pthread_mutex_unlock(&(cls->client_set_lock));

	// we are done now if the shutdown was called, or if we already have a stream popped from the active client connections
	if(was_shutdown_called || strm != NULL)
		return strm;
	// else if we were allowed to create a client connection, then do it
	else if(create_client_allowed)
	{
		strm = create_client_connection(cls);

		// if a stream was not created, then
		// we decrement the curr_client_count, and wake up any thread that is waiting for shutting down the client set
		if(strm == NULL)
		{
			pthread_mutex_lock(&(cls->client_set_lock));

			// decrement the curr_client_count
			cls->curr_client_count--;

			// wake up a thread that wants shutdown and is waiting for all clients to be destroyed
			if(cls->shutdown_called && cls->curr_client_count == 0)
				pthread_cond_signal(&(cls->client_count_reached_0_after_shutdown));

			pthread_mutex_unlock(&(cls->client_set_lock));
		}

		// here we return the created stream OR NULL, if not created
		return strm;
	}
	// return NULL, if it was just a plain simple timeout
	else
		return NULL;
}

void return_client(client_set* cls, stream* strm)
{
	int client_to_be_destroyed = 0;

	pthread_mutex_lock(&(cls->client_set_lock));

	// these are the conditions -> to destroy the client
	if(strm->error || cls->shutdown_called || cls->curr_client_count > cls->max_client_count)
	{
		// decrement the curr_client_count
		cls->curr_client_count--;

		// wake up a thread that wants shutdown and is waiting for all clients to be destroyed
		if(cls->shutdown_called && cls->curr_client_count == 0)
			pthread_cond_signal(&(cls->client_count_reached_0_after_shutdown));

		client_to_be_destroyed = 1;
	}
	// else push the ctream to active clients queue
	else
	{
		push_to_stream_queue(cls, strm);

		// signal a thread that are waiting for no clients in the active_clients_queue
		pthread_cond_signal(&(cls->all_clients_in_use_at_max_clients));
	}

	pthread_mutex_unlock(&(cls->client_set_lock));

	// if the client connection was suppossed to be destroyed, then we do it here, outside the lock
	if(client_to_be_destroyed)
		destroy_client_connection(cls, strm);
}

void shutdown_and_delete_client_set(client_set* cls)
{
	// shutdown logic
	pthread_mutex_lock(&(cls->client_set_lock));

	// signal all threads that are waiting for no clients in the active_clients_queue
	// to let them know that a shutdown was called
	pthread_cond_broadcast(&(cls->all_clients_in_use_at_max_clients));

	// set shutdown_Called and wait until curr_cient_count > 0 
	cls->shutdown_called = 1;
	while(cls->curr_client_count > 0)
		pthread_cond_wait(&(cls->client_count_reached_0_after_shutdown), &(cls->client_set_lock));

	pthread_mutex_unlock(&(cls->client_set_lock));

	// delete logic
	cls->ssl_ctx = NULL;
	deinitialize_queue(&(cls->active_clients_queue));
	pthread_mutex_destroy(&(cls->client_set_lock));
	pthread_cond_destroy(&(cls->all_clients_in_use_at_max_clients));
	pthread_cond_destroy(&(cls->client_count_reached_0_after_shutdown));
	free(cls);
}