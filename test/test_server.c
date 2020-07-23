#include<connection_group.h>
#include<server.h>

#include<string.h>
#include<signal.h>

volatile int listen_fd = -1;

void intHandler(int dummy)
{
	printf("interrupt ctrl + c\n");
	if(listen_fd != -1)
	{
		printf("closing listenning socket\n");
    	server_stop(listen_fd);
    }
}

void connection_handler(int conn_fd, void* additional_params);
void datagram_handler(int serv_fd, void* additional_params);

connection_group cgp;

int main()
{
	signal(SIGINT, intHandler);

	cgp = get_connection_group_tcp_ipv4("127.0.0.1", 6969);
	serve(&cgp, NULL, connection_handler, 10, &listen_fd);

	//cgp = get_connection_group_udp_ipv4("127.0.0.1", 6969);
	//serve(&cgp, NULL, datagram_handler, 10, &listen_fd);

	//cgp = get_connection_group_tcp_ipv6("::1", 6969);
	//serve(&cgp, NULL, connection_handler, 10, &listen_fd);

	//cgp = get_connection_group_udp_ipv6("::1", 6969);
	//serve(&cgp, NULL, datagram_handler, 10, &listen_fd);

	return 0;
}

int process(char* buffer)
{
	printf("received : %s length %lu\n", buffer, strlen(buffer));

	if(strcmp(buffer, "exit\r\n") == 0 || strcmp(buffer, "exit\n") == 0 || strcmp(buffer, "exit") == 0)
	{
		strcpy(buffer, "xit\r\n");
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
void connection_handler(int conn_fd, void* additional_params)
{
	printf("TCP Connection : %d\n", conn_fd);
	char buffer[1000];

	int buffreadlength = -1;
	int buffsentlength = -1;

	while(1)
	{
		buffreadlength = recv(conn_fd, buffer, 999, 0);
		if(buffreadlength == -1 || buffreadlength == 0)
		{
			break;
		}

		buffer[buffreadlength] = '\0';

		// process the buffer here
		int process_result = process(buffer);

		buffreadlength = strlen(buffer);
		buffsentlength = send(conn_fd, buffer, buffreadlength, 0);
		if(buffsentlength == -1 || buffsentlength == 0)
		{
			break;
		}

		if(process_result != 0)
		{
			break;
		}
	}
}

// for udp
void datagram_handler(int serv_fd, void* additional_params)
{
	printf("UDP Server : %d\n", serv_fd);
	char buffer[1000];

	while(1)
	{
		struct sockaddr_in6 cliaddr;
		socklen_t cliaddrlen = (cgp.ADDRESS.sa_family == AF_INET6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
		int buffreadlength = recvfrom(serv_fd, buffer, 999, 0, (struct sockaddr *) &cliaddr, &cliaddrlen);
		if(buffreadlength == -1 || buffreadlength == 0)
		{
			break;
		}

		buffer[buffreadlength] = '\0';

		// process the buffer here
		int process_result = process(buffer);

		buffreadlength = strlen(buffer);
		int buffsentlength = sendto(serv_fd, buffer, buffreadlength, 0, (struct sockaddr *) &cliaddr, cliaddrlen);
		if(buffsentlength == -1 || buffsentlength == 0)
		{
			break;
		}

		if(process_result != 0)
		{
			break;
		}
	}
}
