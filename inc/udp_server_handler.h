#ifndef UDP_SERVER_HANDLER_H
#define UDP_SERVER_HANDLER_H

int udp_server_handler(int listen_fd, void (*handler)(int conn_fd), unsigned int thread_count);

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