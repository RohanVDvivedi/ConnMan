#include<server.h>

int serve(connection_group* conn_grp_p, void (*handler)(int conn_fd), volatile int* listen_fd_p)
{
	int err;

	// phase 1
	// file discriptor to socket
	err = socket(conn_grp_p->ADDRESS.sin_family, conn_grp_p->PROTOCOL, 0);
	if(err == -1)
	{
		goto end;
	}
	int listen_fd = err;
	(*listen_fd_p) = err;

	struct sockaddr_in server_addr = conn_grp_p->ADDRESS;

	// phase 2
	// bind server address struct with the file discriptor
	err = bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err == -1)
	{
		goto end;
	}

	// go to respective function based on TRANSMISSION_PROTOCOL_TYPE
	if(conn_grp_p->PROTOCOL == SOCK_STREAM)			// tcp
	{
		err = tcp_server_handler(listen_fd, handler);
	}
	else if(conn_grp_p->PROTOCOL == SOCK_DGRAM)		// udp
	{
		err = udp_server_handler(listen_fd, handler);
	}
	else
	{
		err = -1;
	}
	if (err == -1)
	{
		goto end;
	}

	return 0;
	end: return err;
}

int server_stop(int listen_fd)
{
	// phase 6
	// closing server socket
	int err = close(listen_fd);
	if (err == -1)
	{
		goto end;
	}
	return 0;
	end: return err;
}