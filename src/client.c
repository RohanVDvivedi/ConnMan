#include<client.h>

int connect_to(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, void (*handler)(int fd))
{
	int err;

	// then we try to set up socket and retrieve the file discriptor that is returned
	err = socket(ADDRESS_FAMILY, TRANSMISSION_PROTOCOL_TYPE, 0);
    if(err == -1)
    {
    	goto end;
    }
    int fd = err;

    // then we set up socket address with the address received from the host using the get host name function 
	struct sockaddr_in server_addr;
	server_addr.sin_family = ADDRESS_FAMILY;
	server_addr.sin_addr.s_addr = htonl(SERVER_ADDRESS);
	server_addr.sin_port = htons(PORT);

	// next we try and attempt to connect the socket formed whose file discriptor we have
	// to connect using the address that we have in sockaddr_in struct in server_addr
	err = connect(fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
	if(err == -1)
	{
        goto end;
	}

	// pass the file discriptor to the handler, so that request can be handled
	handler(fd);

	err = close(fd);
	if(err == -1)
    {
    	goto end;
    }

	return 0;

	end: return err;
}

int connect_to_tcp_on_ipv4(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd))
{
	return connect_to(AF_INET, SOCK_STREAM,
			SERVER_ADDRESS, PORT,
			connection_handler);
}

int connect_to_tcp_on_ipv6(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd))
{
	return connect_to(AF_INET6, SOCK_STREAM,
			SERVER_ADDRESS, PORT,
			connection_handler);
}

int connect_to_udp_on_ipv4(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd))
{
	return connect_to(AF_INET, SOCK_DGRAM,
			SERVER_ADDRESS, PORT,
			connection_handler);
}

int connect_to_udp_on_ipv6(uint32_t SERVER_ADDRESS, uint16_t PORT, void (*connection_handler)(int conn_fd))
{
	return connect_to(AF_INET6, SOCK_DGRAM,
			SERVER_ADDRESS, PORT,
			connection_handler);
}