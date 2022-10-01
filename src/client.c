#include<client.h>

#include<sys/socket.h>

// the sub functions, that make up the connect_to function

// returns file-discriptor to the socket, through which client connection has been made
int make_connection(comm_address* server_addr_p)
{
	// then we try to set up socket and retrieve the file discriptor that is returned
	int err = socket(server_addr_p->ADDRESS.sa_family, server_addr_p->PROTOCOL, 0);
    if(err == -1)
    	return err;
    int fd = err;

	// next we try and attempt to connect the socket formed whose file discriptor we have
	// to connect using the address that we have in sockaddr_in struct in server_addr
	if(server_addr_p->ADDRESS.sa_family == AF_INET)
		err = connect(fd, &(server_addr_p->ADDRESS), sizeof(server_addr_p->ADDRESS_ipv4));
	else if(server_addr_p->ADDRESS.sa_family == AF_INET6)
		err = connect(fd, &(server_addr_p->ADDRESS), sizeof(server_addr_p->ADDRESS_ipv6));
	if(err == -1)
		return err;

	return fd;
}