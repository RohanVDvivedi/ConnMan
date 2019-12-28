#include<udp_server_handler.h>

int udp_server_handler(int listen_fd, void (*handler)(int conn_fd))
{
	// there can be errors anywhere at any point
	int err;

	// we do not assign new threads here in UDP server,
	// it is not connection oriented,
	// hence we connect on the same file discriptor as we listen on tcp
	handler(listen_fd);

	return 0;

	end: return err;
}