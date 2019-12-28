#include<server.h>

typedef struct handler_wrapper_input_params handler_wrapper_input_params;
struct handler_wrapper_input_params
{
	int conn_fd;
	void (*handler)(int conn_fd);
};

handler_wrapper_input_params* get_new_handler_wrapper_input_params(int conn_fd, void (*handler)(int conn_fd))
{
	handler_wrapper_input_params* handler_wrapper_input_params_p = (handler_wrapper_input_params*)malloc(sizeof(handler_wrapper_input_params));
	handler_wrapper_input_params_p->conn_fd = conn_fd;
	handler_wrapper_input_params_p->handler = chandler;
	return handler_wrapper_input_params_p;
}

void* handler_wrapper(void* handler_wrapper_input_params_v_p)
{
	handler_wrapper_input_params* handler_data = ((handler_wrapper_input_params*)handler_wrapper_input_params_v_p);
	handler_data->connection_handler(handler_data->conn_fd);

	// phase 5
	// closing client socket
	close(handler_data->conn_fd);

	free(handler_data);
	return NULL;
}

int tcp_loop(int server_socket_fd, void (*connection_handler)(int fd));

int udp_loop(int server_socket_fd, void (*datagram_handler)(int fd));

server* serve(connection_group* conn_grp_p, void (*handler)(int conn_fd))
{
	server* server_p = (server*) malloc(sizeof(server));
	server_p->group = conn_grp_p;
	server_p->handler = handler;

	int err;

	// phase 1
	// file discriptor to socket
	err = socket(server_p->group->ADDRESS_FAMILY, server_p->group->TRANSMISSION_PROTOCOL_TYPE, 0);
	if(err == -1)
	{
		goto end;
	}
	server_p->listenfd = err;

	struct sockaddr_in server_addr;
	server_addr.sin_family = server_p->group->ADDRESS_FAMILY;
	server_addr.sin_addr.s_addr = htonl(server_p->group->SERVER_ADDRESS);
	server_addr.sin_port = htons(server_p->group->PORT);

	// phase 2
	// bind server address struct with the file discriptor
	err = bind(server_p->listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err == -1)
	{
		goto end;
	}

	// go to respective function based on TRANSMISSION_PROTOCOL_TYPE
	if(server_p->group->TRANSMISSION_PROTOCOL_TYPE == SOCK_STREAM)			// tcp
	{
		server_p->keepTcpServerListenning = 1;
		err = tcp_loop(server_p);
	}
	else if(server_p->group->TRANSMISSION_PROTOCOL_TYPE == SOCK_DGRAM)		// udp
	{
		err = udp_loop(server_p);
	}
	else
	{
		err = -1;
	}
	if (err == -1)
	{
		goto end;
	}

	return server_p;
	free(server_p);
	end: return NULL;
}

int server_stop(server* server_p)
{
	// phase 6
	// closing server socket
	int err = close(server_p->listenfd);
	free(server_p);
	if (err == -1)
	{
		goto end;
	}
	return 0;
	end: return err;
}

int tcp_loop(server* server_p)
{
	// there can be errors anywhere at any point
	int err;

	// phase 3
	// listenning on the socket file discriptor 
	err = listen(server_p->listenfd, DEFAULT_BACKLOG_QUEUE_SIZE);
	if (err == -1)
	{
		goto end;
	}

	// start accepting in loop
	struct sockaddr_in client_addr;		socklen_t client_len = sizeof(client_addr);
	while(server_p->keepTcpServerListenning)
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
		submit(server_p->group->thread_pool, connection_handler_wrapper, get_new_connection_handler_wrapper_input_params(conn_fd, server_p->handler));
	}

	return 0;

	end: return err;
}