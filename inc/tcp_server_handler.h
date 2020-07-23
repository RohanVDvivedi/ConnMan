#ifndef TCP_HANDLER_H
#define TCP_HANDLER_H

#include<netinet/in.h>
#include<unistd.h>

#include<errno.h>

#include<executor.h>

#define DEFAULT_BACKLOG_QUEUE_SIZE 10											// we do not ask the operating system to queue any more than 10 awaiting connections
#define DEFAULT_NO_CONNECTION_THREAD_DESTROY_TIMEOUT_IN_MICRO_SECONDS 3000 * 1000	// no new connection for 3 seconds and the waiting thread dies

int tcp_server_handler(int listen_fd, void* additional_params, void (*handler)(int conn_fd, void* additional_params), unsigned int thread_count);

/*

TCP connection handler
do not close any file discriptors here,
just return when you want to exit the current client connection and close it
server does not shutdown if you return from the connection handler,
you return from here to exit this client's connection only, 
other clients may be still being served

each connection handler is called, with an idea that it will only serve this connection and die

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

*/

#endif