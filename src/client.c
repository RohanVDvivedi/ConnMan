#include<connman/client.h>

#include<connman/ssl_stream.h>
#include<cutlery/stream_for_file_descriptor.h>

#include<sys/socket.h>
#include<unistd.h>

// the sub functions, that make up the connect_to function

// returns file-discriptor to the socket, through which client connection has been made
int make_connection(comm_address* server_addr_p, comm_address* client_addr_p, uint64_t timeout_in_milliseconds)
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

	if(timeout_in_milliseconds != BLOCKING && timeout_in_milliseconds != NON_BLOCKING)
	{
		struct timeval tv = timespec_to_timeval(timespec_from_milliseconds(timeout_in_milliseconds));
		setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
		setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
	}

	return fd;
}

int make_connection_stream(stream* strm, comm_address* server_addr_p, const char* hostname, comm_address* client_addr_p, SSL_CTX* ssl_ctx, uint64_t timeout_in_milliseconds)
{
	int fd = make_connection(server_addr_p, client_addr_p, timeout_in_milliseconds);
	if(fd == -1)
		return -1;

	if(ssl_ctx == NULL)
	{
		if(!initialize_stream_for_fd(strm, fd))
		{
			close(fd);
			return -1;
		}
		return fd;
	}
	else
	{
		if(!initialize_stream_for_ssl_client(strm, ssl_ctx, hostname, fd))
		{
			close(fd);
			return -1;
		}
		return fd;
	}
}