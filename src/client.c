#include<client.h>

#include<sys/socket.h>
#include<unistd.h>

// the sub functions, that make up the connect_to function

// returns file-discriptor to the socket, through which client connection has been made
int make_connection(comm_address* server_addr_p, comm_address* client_addr_p)
{
	// then we try to set up socket and retrieve the file discriptor that is returned
	int err = socket(server_addr_p->ADDRESS.sa_family, server_addr_p->PROTOCOL, 0);
    if(err == -1)
    	return err;
    int fd = err;

	// bind client address struct with the file descriptor, if a specific client address is provided
	if(client_addr_p)
	{
		int err = bind(fd, &(client_addr_p->ADDRESS), get_sockaddr_size(client_addr_p));
		if(err)
		{
			close(fd);
			return err;
		}
	}

	// next we try and attempt to connect the socket formed whose file discriptor we have
	// to connect using the address that we have in sockaddr_in struct in server_addr
	// connect call is noop for a udp socket
	err = connect(fd, &(server_addr_p->ADDRESS), get_sockaddr_size(server_addr_p));
	if(err == -1)
	{
		close(fd);
		return err;
	}

	return fd;
}