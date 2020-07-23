#include<server.h>

int serve(connection_group* conn_grp_p, void* additional_params, void (*handler)(int conn_fd, void* additional_params), unsigned int thread_count, volatile int* listen_fd_p)
{
	if(!thread_count)
		return INVALID_THREAD_COUNT;

	// phase 1
	// file discriptor to socket
	int err = socket(conn_grp_p->ADDRESS.sa_family, conn_grp_p->PROTOCOL, 0);
	if(err == -1)
		return err;
	int listen_fd = err;
	(*listen_fd_p) = err;

	// phase 2
	// bind server address struct with the file discriptor
	if(conn_grp_p->ADDRESS.sa_family == AF_INET)
		err = bind(listen_fd, &(conn_grp_p->ADDRESS), sizeof(conn_grp_p->ADDRESS_ipv4));
	else if(conn_grp_p->ADDRESS.sa_family == AF_INET6)
		err = bind(listen_fd, &(conn_grp_p->ADDRESS), sizeof(conn_grp_p->ADDRESS_ipv6));
	else
		return INVALID_ADDRESS_FAMILY;
	if(err == -1)
		return err;

	// go to respective function based on TRANSMISSION_PROTOCOL_TYPE
	if(conn_grp_p->PROTOCOL == SOCK_STREAM)			// tcp
		return tcp_server_handler(listen_fd, additional_params, handler, thread_count);
	else if(conn_grp_p->PROTOCOL == SOCK_DGRAM)		// udp
		return udp_server_handler(listen_fd, additional_params, handler, thread_count);
	else
		return INVALID_PROTOCOL;
}

int server_stop(int listen_fd)
{
	// phase 6
	// closing server socket
	return close(listen_fd);
}