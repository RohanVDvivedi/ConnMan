#include<server.h>

int serve(connection_group* conn_grp_p, void (*handler)(int conn_fd), unsigned int thread_count, volatile int* listen_fd_p)
{
	int err;

	if(!thread_count)
		return INVALID_THREAD_COUNT;

	// phase 1
	// file discriptor to socket
	err = socket(conn_grp_p->ADDRESS.sin_family, conn_grp_p->PROTOCOL, 0);
	if(err == -1)
		return err;
	int listen_fd = err;
	(*listen_fd_p) = err;

	struct sockaddr_in server_addr = conn_grp_p->ADDRESS;

	// phase 2
	// bind server address struct with the file discriptor
	err = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err == -1)
		return err;

	// go to respective function based on TRANSMISSION_PROTOCOL_TYPE
	if(conn_grp_p->PROTOCOL == SOCK_STREAM)			// tcp
		return tcp_server_handler(listen_fd, handler, thread_count);
	else if(conn_grp_p->PROTOCOL == SOCK_DGRAM)		// udp
		return udp_server_handler(listen_fd, handler, thread_count);
	else
		return INVALID_PROTOCOL;
}

int server_stop(int listen_fd)
{
	// phase 6
	// closing server socket
	return close(listen_fd);
}