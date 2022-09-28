#include<server.h>

#include<sys/socket.h>	// socket, bind
#include<unistd.h>		// close

#include<tcp_server_handler.h>
#include<udp_server_handler.h>

static int make_server_ready_to_listen(connection_group* conn_grp_p)
{
	// phase 1
	// file descriptor to socket
	int listen_fd = socket(conn_grp_p->ADDRESS.sa_family, conn_grp_p->PROTOCOL, 0);
	if(listen_fd == -1)
		return -1;

	// phase 2
	// bind server address struct with the file descriptor
	if(conn_grp_p->ADDRESS.sa_family == AF_INET)
		return bind(listen_fd, &(conn_grp_p->ADDRESS), sizeof(conn_grp_p->ADDRESS_ipv4));
	else if(conn_grp_p->ADDRESS.sa_family == AF_INET6)
		return bind(listen_fd, &(conn_grp_p->ADDRESS), sizeof(conn_grp_p->ADDRESS_ipv6));
	else
	{
		close(listen_fd);
		return -2;
	}
}

int serve_using_handlers(connection_group* conn_grp_p, void* additional_params, void (*handler)(int conn_fd, void* additional_params), unsigned int thread_count, volatile int* listen_fd_p)
{
	if(!thread_count)
		return -1;

	*listen_fd_p = make_server_ready_to_listen(conn_grp_p);
	if(*listen_fd_p < 0)
		return *listen_fd_p;

	// go to respective function based on TRANSMISSION_PROTOCOL_TYPE
	if(conn_grp_p->PROTOCOL == SOCK_STREAM)			// tcp
		return tcp_server_handler(*listen_fd_p, additional_params, handler, thread_count);
	else if(conn_grp_p->PROTOCOL == SOCK_DGRAM)		// udp
		return udp_server_handler(*listen_fd_p, additional_params, handler, thread_count);
	else
		return -3;
}

int server_stop(int listen_fd)
{
	// phase 6
	// closing server socket
	return close(listen_fd);
}