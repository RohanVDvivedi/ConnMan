#ifndef TCP_HANDLER_H
#define TCP_HANDLER_H

#include<netinet/in.h>
#include<unistd.h>

#include<errno.h>

#include<executor.h>

#define DEFAULT_MAXIMUM_CONNECTIONS 200											// maximum 200 threads for IO (1 thread per connection basis)
#define DEFAULT_BACKLOG_QUEUE_SIZE 10											// we do not ask the operating system to queue any more than 10 awaiting connections
#define DEFAULT_NO_CONNECTION_THREAD_DESTROY_TIMEOUT_IN_MICRO_SECONDS 3000 * 1000	// no new connection for 3 seconds and the waiting thread dies

int tcp_server_handler(int listen_fd, void (*handler)(int conn_fd));

// Examples for 
/*
TCP connection handler
do not close any file discriptors here, just return when you want to exit the current client connection and close it
server does not shutdown by returning, from here, you return from here to exit this client connection only, other clients may be still being served

void (*connection_handler)(int conn_fd)
{
	char buffer[1000];
	while(1)
	{
		int buffreadlength = recv(conn_fd, buffer, 999, 0);
		if(buffreadlength == -1)
		{
			break;
		}

		buffer[buffreadlength] = '\0';

		// process the buffer here

		buffreadlength = strlen(buffer);
		send(conn_fd, buffer, buffreadlength, 0);
	}
}

UDP datagram handler
do not close any file discriptors here, just return when you want to exit the server, and stop serving altogether

void (*datagram_handler)(int serv_fd)
{
	char buffer[1000];
	while(1)
	{
		struct sockaddr_in cliaddr; socklen_t cliaddrlen = sizeof(cliaddr);
		int buffreadlength = recvfrom(serv_fd, buffer, 999, 0, &cliaddr, &cliaddrlen);
		if(buffreadlength == -1)
		{
			break;
		}

		buffer[buffreadlength] = '\0';

		// process the buffer here

		buffreadlength = strlen(buffer);
		sendto(serv_fd, buffer, buffreadlength, 0, &cliaddr, cliaddrlen); 
	}
}

*/

#endif