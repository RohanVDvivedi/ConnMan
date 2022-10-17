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

void connection_stream_handler(stream* conn_strm, void* additional_params);

comm_address cgp;

int main()
{
	signal(SIGINT, intHandler);

	cgp = new_comm_address_tcp_ipv4("127.0.0.1", 6969);
	//cgp = new_comm_address_tcp_ipv6("::1", 6969);

	SSL_CTX* ssl_ctx = NULL;

	serve_using_stream_handlers(&cgp, NULL, connection_stream_handler, 10, ssl_ctx, &listen_fd);

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
void connection_stream_handler(stream* conn_strm, void* additional_params)
{
	printf("TCP Connection : %p\n", conn_strm);
	char buffer[1000];

	int buffreadlength = -1;
	int buffsentlength = -1;

	while(1)
	{
		buffreadlength = read_from_stream(conn_strm, buffer, 999);
		if(buffreadlength == -1 || buffreadlength == 0)
		{
			break;
		}

		buffer[buffreadlength] = '\0';

		// process the buffer here
		int process_result = process(buffer);

		buffreadlength = strlen(buffer);
		buffsentlength = write_to_stream(conn_strm, buffer, buffreadlength);
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