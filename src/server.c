#include<connman/server.h>

#include<sys/socket.h>	// socket, bind
#include<unistd.h>		// close

#include<connman/tcp_server_handler.h>
#include<connman/udp_server_handler.h>

#include<cutlery/stream.h>
#include<connman/tcp_server_stream_handler.h>

static int make_server_ready_to_listen(comm_address* server_addr_p)
{
	// phase 1
	// file descriptor to socket
	int listen_fd = socket(server_addr_p->ADDRESS.sa_family, server_addr_p->PROTOCOL, 0);
	if(listen_fd == -1)
		return -1;

	// set socket options so that it allows you to reuse the address and port right after using it once
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));

	// phase 2
	// bind server address struct with the file descriptor
	int err = bind(listen_fd, &(server_addr_p->ADDRESS), get_sockaddr_size(server_addr_p));
	if(err)
	{
		close(listen_fd);
		return -1;
	}

	// on success, return listen_fd
	return listen_fd;
}

#define DEFAULT_MAX_THREAD_COUNT 8

int serve_using_handlers(comm_address* server_addr_p, void* additional_params, void (*handler)(int conn_fd, void* additional_params), unsigned int thread_count, volatile int* listen_fd_p)
{
	if(thread_count == 0)
		thread_count = DEFAULT_MAX_THREAD_COUNT;

	if(server_addr_p->PROTOCOL != SOCK_STREAM && server_addr_p->PROTOCOL != SOCK_DGRAM)
		return -1;

	*listen_fd_p = make_server_ready_to_listen(server_addr_p);
	if(*listen_fd_p < 0)
		return *listen_fd_p;

	int err = -1;
	if(server_addr_p->PROTOCOL == SOCK_STREAM)			// tcp
		err = tcp_server_handler(*listen_fd_p, additional_params, handler, thread_count);
	else if(server_addr_p->PROTOCOL == SOCK_DGRAM)		// udp
		err = udp_server_handler(*listen_fd_p, additional_params, handler, thread_count);

	close(*listen_fd_p);
	return err;
}

int serve_using_stream_handlers(comm_address* server_addr_p, void* additional_params, void (*stream_handler)(stream* strm, void* additional_params), unsigned int thread_count, SSL_CTX* ssl_ctx, volatile int* listen_fd_p)
{
	if(thread_count == 0)
		thread_count = DEFAULT_MAX_THREAD_COUNT;

	if(server_addr_p->PROTOCOL != SOCK_STREAM && server_addr_p->PROTOCOL != SOCK_DGRAM)
		return -1;

	*listen_fd_p = make_server_ready_to_listen(server_addr_p);
	if(*listen_fd_p < 0)
		return *listen_fd_p;

	int err = tcp_server_stream_handler(*listen_fd_p, additional_params, stream_handler, thread_count, ssl_ctx);

	close(*listen_fd_p);
	return err;
}

int server_stop(int listen_fd)
{
	// phase 6
	// closing server socket
	return close(listen_fd);
}