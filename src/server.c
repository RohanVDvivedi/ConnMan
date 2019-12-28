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

int tcp_loop(unsigned long long int BACKLOG_QUEUE_SIZE, int server_socket_fd, void (*connection_handler)(int fd));

int udp_loop(int server_socket_fd, void (*datagram_handler)(int fd));

int serve_default(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, void (*handler)(int fd))
{
	return serve(ADDRESS_FAMILY, TRANSMISSION_PROTOCOL_TYPE, SERVER_ADDRESS, PORT, DEFAULT_BACKLOG_QUEUE_SIZE, handler);
}

int serve(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, unsigned long long int BACKLOG_QUEUE_SIZE, void (*handler)(int fd))
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

	// go to respective function based on TRANSMISSION_PROTOCOL_TYPE
	if(TRANSMISSION_PROTOCOL_TYPE == SOCK_STREAM)			// tcp
	{
		err = tcp_loop(BACKLOG_QUEUE_SIZE, fd, handler);
	}
	else if(TRANSMISSION_PROTOCOL_TYPE == SOCK_DGRAM)		// udp
	{
		err = udp_loop(fd, handler);
	}
	else
	{
		err = -1;
	}
	if (err == -1)
	{
		goto end;
	}

	return fd;

	end: return err;
}

int server_stop(int fd)
{
	int err;

	// phase 6
	// closing server socket
	err = close(fd);
	if (err == -1)
	{
		goto end;
	}

	return 0;

	end: return err;
}

// for a TCP server, it is mandatory for us to stop listenning for new connections
// , and shutdown gracefully, shutting down the thread executors, 
// and free up other resources that we might have hoarded, here is an interrupt handler for this
static volatile int keepServersRunning = 1;
static volatile int* tcpListenningFd = NULL;
void intHandler(int dummy) {
    keepServersRunning = 0;
    if(tcpListenningFd != NULL)
    {
    	// phase 6
		// closing server socket
		close(*tcpListenningFd);
    }
}

int tcp_loop(unsigned long long int BACKLOG_QUEUE_SIZE, int server_socket_fd, void (*connection_handler)(int fd))
{
	// this is the file discriptor we are gonna deal with,
	// until we get the client connection and then we dela with conn_fd
	// the client socket file discriptor
	int fd = server_socket_fd;

	// there can be errors anywhere at any point
	int err;

	// phase 3
	// listenning on the socket file discriptor 
	err = listen(fd, BACKLOG_QUEUE_SIZE);
	if (err == -1)
	{
		goto end;
	}

	// we set up an executor to handle every accepted connection on other threads as a job
	executor* executor_p = get_executor(CACHED_THREAD_POOL_EXECUTOR, DEFAULT_MAXIMUM_THREADS_IO_HANDLER, DEFAULT_NO_CONNECTION_THREAD_DESTROY_TIMEOUT_IN_MICRO_SECONDS); 
	
	// start accepting in loop
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	while(keepServersRunning)
	{
		// phase 4
		// accept uses backlog queue connection and de-queues them 
		err = accept(fd, (struct sockaddr*)&client_addr, &client_len);
		if (err == -1)
		{
			continue;
		}
		int conn_fd = err;

		// serve the connection that has been accepted, submit it to executor, to assign a thread to it
		submit(executor_p, connection_handler_wrapper, get_new_connection_handler_wrapper_input_params(conn_fd, connection_handler));
	}

	// shutdown and delete the executor you created for the loop, 
	// do not shutdown immediately, we shutdown only after the current on going requests have been handled gracefully
	shutdown_executor(executor_p, 1);
	wait_for_all_threads_to_complete(executor_p);
	delete_executor(executor_p);

	return 0;

	end: return err;
}

int udp_loop(int server_socket_fd, void (*datagram_handler)(int fd))
{
	// this is the file discriptor we are gonna del with,
	// until we get the client connection and then we dela with conn_fd
	// the client socket file discriptor
	int fd = server_socket_fd;

	// there can be errors anywhere at any point
	int err;

	// we do not assign new threads here in UDP server,
	// in tcp it is required, because it is connection oriented, hence you get different file discriptor for different clients
	datagram_handler(fd);

	return 0;

	end: return err;
}

int serve_tcp_on_ipv4(uint16_t PORT, void (*connection_handler)(int conn_fd))
{
	return serve_default(AF_INET, SOCK_STREAM,
			INADDR_ANY, PORT, connection_handler);
}

int serve_tcp_on_ipv6(uint16_t PORT, void (*connection_handler)(int conn_fd))
{
	return serve_default(AF_INET6, SOCK_STREAM,
			INADDR_ANY, PORT, connection_handler);
}

int serve_udp_on_ipv4(uint16_t PORT, void (*datagram_handler)(int serv_fd))
{
	return serve_default(AF_INET, SOCK_DGRAM,
			INADDR_ANY, PORT, datagram_handler);
}

int serve_udp_on_ipv6(uint16_t PORT, void (*datagram_handler)(int serv_fd))
{
	return serve_default(AF_INET6, SOCK_DGRAM,
			INADDR_ANY, PORT, datagram_handler);
}
