#include<comm_address.h>
#include<server.h>

#include<stdio.h>
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

comm_address cgp;

int main()
{
	signal(SIGINT, intHandler);

	cgp = new_comm_address_tcp_ipv4("127.0.0.1", 6969);
	//cgp = new_comm_address_udp_ipv4("127.0.0.1", 6969);
	//cgp = new_comm_address_tcp_ipv6("::1", 6969);
	//cgp = new_comm_address_udp_ipv6("::1", 6969);

	serve_using_handlers(&cgp, NULL, connection_handler, 10, &listen_fd);

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
		comm_address cliaddr = cgp;
		socklen_t cliaddrlen = get_sockaddr_size(&cliaddr);
		int buffreadlength = recvfrom(serv_fd, buffer, 999, 0, &(cliaddr.ADDRESS), &cliaddrlen);
		if(buffreadlength == -1 || buffreadlength == 0)
		{
			break;
		}

		buffer[buffreadlength] = '\0';

		// process the buffer here
		int process_result = process(buffer);

		buffreadlength = strlen(buffer);
		int buffsentlength = sendto(serv_fd, buffer, buffreadlength, 0, &(cliaddr.ADDRESS), cliaddrlen);
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
