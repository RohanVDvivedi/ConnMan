#include<server.h>

typedef struct connection_handler_wrapper_input_params connection_handler_wrapper_input_params;
struct connection_handler_wrapper_input_params
{
	int conn_fd;
	void (*connection_handler)(int conn_fd);
};

connection_handler_wrapper_input_params* get_new_connection_handler_wrapper_input_params(int conn_fd, void (*connection_handler)(int conn_fd))
{
	connection_handler_wrapper_input_params* connection_handler_wrapper_input_params_p = (connection_handler_wrapper_input_params*)malloc(sizeof(connection_handler_wrapper_input_params));
	connection_handler_wrapper_input_params_p->conn_fd = conn_fd;
	connection_handler_wrapper_input_params_p->connection_handler = connection_handler;
	return connection_handler_wrapper_input_params_p;
}

void* connection_handler_wrapper(void* connection_handler_wrapper_input_params_v_p)
{
	connection_handler_wrapper_input_params* handler_data = ((connection_handler_wrapper_input_params*)connection_handler_wrapper_input_params_v_p);
	handler_data->connection_handler(handler_data->conn_fd);

	// phase 5
	// closing client socket
	close(handler_data->conn_fd);

	free(handler_data);
	return NULL;
}

int serve(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, unsigned long long int BACKLOG_QUEUE_SIZE, void (*connection_handler)(int conn_fd))
{
	int err;

	// phase 1
	// file discriptor to socket
	err = socket(ADDRESS_FAMILY, TRANSMISSION_PROTOCOL_TYPE, 0);
	if(err == -1)
	{
		goto end;
	}
	int fd = err;

	struct sockaddr_in server_addr;
	server_addr.sin_family = ADDRESS_FAMILY;
	server_addr.sin_addr.s_addr = htonl(SERVER_ADDRESS);
	server_addr.sin_port = htons(PORT);

	// phase 2
	// bind server address struct with the file discriptor
	err = bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err == -1)
	{
		goto end;
	}

	// phase 3
	// listenning on the socket file discriptor 
	err = listen(fd, BACKLOG_QUEUE_SIZE);
	if (err == -1)
	{
		goto end;
	}

	// we set up an executor to handle every accepted connection on other threads as a job
	executor* executor_p = get_executor(CACHED_THREAD_POOL_EXECUTOR, MAXIMUM_THREADS_IO_HANDLER, NO_CONNECTION_THREAD_DESTROY_TIMEOUT_IN_MICRO_SECONDS); 
	
	// start accepting in loop
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int conn_fd;
	while(1)
	{
		// phase 4
		// accept uses backlog queue connection and de-queues them 
		err = accept(fd, (struct sockaddr*)&client_addr, &client_len);
		if (err == -1)
		{
			continue;
		}
		conn_fd = err;

		// serve the connection that has been accepted
		submit(executor_p, connection_handler_wrapper, get_new_connection_handler_wrapper_input_params(conn_fd, connection_handler));
	}

	// shutdown and delete the executor you created for the loop, 
	// do not shutdown immediately, we shutdown only after the current on going requests have been handled gracefully
	shutdown_executor(executor_p, 0);
	wait_for_all_threads_to_complete(executor_p);
	delete_executor(executor_p);

	// phase 6
	// closing server socket
	err = close(fd);
	if (err == -1)
	{
		goto end;
	}

	end: return err;
}

int serve_tcp_on_ipv4(uint16_t PORT, void (*connection_handler)(int conn_fd))
{
	return serve(AF_INET, SOCK_STREAM,
			INADDR_ANY, PORT,
			DEFAULT_BACKLOG_QUEUE_SIZE,
			connection_handler);
}

int serve_tcp_on_ipv6(uint16_t PORT, void (*connection_handler)(int conn_fd))
{
	return serve(AF_INET6, SOCK_STREAM,
			INADDR_ANY, PORT,
			DEFAULT_BACKLOG_QUEUE_SIZE,
			connection_handler);
}

int serve_udp_on_ipv4(uint16_t PORT, void (*connection_handler)(int conn_fd))
{
	return serve(AF_INET, SOCK_DGRAM,
			INADDR_ANY, PORT,
			DEFAULT_BACKLOG_QUEUE_SIZE,
			connection_handler);
}

int serve_udp_on_ipv6(uint16_t PORT, void (*connection_handler)(int conn_fd))
{
	return serve(AF_INET6, SOCK_DGRAM,
			INADDR_ANY, PORT,
			DEFAULT_BACKLOG_QUEUE_SIZE,
			connection_handler);
}
