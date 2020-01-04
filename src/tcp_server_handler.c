#include<tcp_server_handler.h>

typedef struct handler_wrapper_input_params handler_wrapper_input_params;
struct handler_wrapper_input_params
{
	int conn_fd;
	void (*handler)(int conn_fd);

	// this is the hashmap in which we have to explicitly tell on which file discriptor we will be working
	connection_thread_pool_manager* manager;
};

handler_wrapper_input_params* get_new_handler_wrapper_input_params(int conn_fd, void (*handler)(int conn_fd), connection_thread_pool_manager* manager)
{
	handler_wrapper_input_params* handler_wrapper_input_params_p = (handler_wrapper_input_params*)malloc(sizeof(handler_wrapper_input_params));
	handler_wrapper_input_params_p->conn_fd = conn_fd;
	handler_wrapper_input_params_p->handler = handler;
	handler_wrapper_input_params_p->manager = manager;
	return handler_wrapper_input_params_p;
}

void* handler_wrapper(void* handler_wrapper_input_params_v_p)
{
	handler_wrapper_input_params* handler_data = ((handler_wrapper_input_params*)handler_wrapper_input_params_v_p);
	
	// make the connection_thread_pool_manager aware about which file_discriptor, we will be working with
	insert_mapping_with_current_thread(handler_data->manager, handler_data->conn_fd);

	handler_data->handler(handler_data->conn_fd);

	// remove your own self from this connection_thread_pool_manager's mapping
	remove_mapping_for_current_thread(handler_data->manager);

	// phase 5
	// closing client socket
	close(handler_data->conn_fd);

	free(handler_data);
	return NULL;
}

int tcp_server_handler(int listen_fd, void (*handler)(int conn_fd))
{
	// there can be errors anywhere at any point
	int err;

	// phase 3
	// listenning on the socket file discriptor 
	err = listen(listen_fd, DEFAULT_BACKLOG_QUEUE_SIZE);
	if (err == -1)
	{
		goto end;
	}

	// start accepting in loop
	struct sockaddr_in client_addr;		socklen_t client_len = sizeof(client_addr);
	connection_thread_pool_manager* manager = get_cached_connection_thread_pool_manager(DEFAULT_MAXIMUM_CONNECTIONS, handler_wrapper);
	while(1)
	{
		// phase 4
		// accept uses backlog queue connection and de-queues them 
		err = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
		if (err == -1)
		{
			// break the listenning loop, if the listen_fd file discriptor is closed
			if(errno == EBADF || errno == ECONNABORTED || errno == EINVAL || errno == EINVAL || errno == ENOTSOCK || errno == EPERM)
			{
				break;
			}
			else
			{
				continue;
			}
		}
		int conn_fd = err;

		// serve the connection that has been accepted, submit it to executor, to assign a thread to it
		submit_job_parameters(manager, get_new_handler_wrapper_input_params(conn_fd, handler, manager));
	}

	close_all_connections_and_wait_for_shutdown(manager);

	delete_connection_thread_pool_manager(manager);

	return 0;

	end: return err;
}
