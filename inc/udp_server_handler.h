#ifndef UDP_SERVER_HANDLER_H
#define UDP_SERVER_HANDLER_H

#include<executor.h>

int udp_server_handler(int listen_fd, void (*handler)(int conn_fd), unsigned int thread_count);

/*

UDP datagram handler
do not close any file discriptors here,
just return when you want to exit the server thread
The server thread will not be recreated, hence the handler must be a while(1) loop

void (*datagram_handler)(int serv_fd)
{
	char buffer[1024];
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