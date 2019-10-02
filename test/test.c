#include<server.h>

#include<string.h>

void connection_handler(int conn_fd);
void datagram_handler(int serv_fd);

int main()
{
	//serve_tcp_on_ipv4(6969, connection_handler);
	serve_udp_on_ipv4(6969, datagram_handler);
	return 0;
}

int process(char* buffer)
{
	printf("received : %s length %lu\n", buffer, strlen(buffer));

	if(strcmp(buffer, "exit\r\n") == 0 || strcmp(buffer, "exit\n") == 0 || strcmp(buffer, "exit") == 0)
	{
		return -1;
	}
	else if(strcmp(buffer, "ping\r\n") == 0 || strcmp(buffer, "ping\n") == 0 || strcmp(buffer, "ping") == 0)
	{
		strcpy(buffer, "pong\r\n");
	}
	else if(strcmp(buffer, "pong\r\n") == 0 || strcmp(buffer, "pong\n") == 0 || strcmp(buffer, "pong") == 0)
	{
		strcpy(buffer, "ping\r\n");
	}

	return 0;
}

// for tcp
void connection_handler(int conn_fd)
{
	printf("TCP Connection : %d\n", conn_fd);
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
		if(process(buffer) != 0)
		{
			break;
		}

		buffreadlength = strlen(buffer);
		send(conn_fd, buffer, buffreadlength, 0);
	}
}

// for udp
void datagram_handler(int serv_fd)
{
	printf("UDP Server : %d\n", serv_fd);
	char buffer[1000];

	while(1)
	{
		struct sockaddr_in cliaddr; socklen_t cliaddrlen = sizeof(cliaddr);
		int buffreadlength = recvfrom(serv_fd, buffer, 999, 0, (struct sockaddr *) &cliaddr, &cliaddrlen);
		buffer[buffreadlength] = '\0';

		// process the buffer here
		if(process(buffer) != 0)
		{
			break;
		}

		buffreadlength = strlen(buffer);
		sendto(serv_fd, buffer, buffreadlength, 0, (struct sockaddr *) &cliaddr, cliaddrlen);
	}
}
