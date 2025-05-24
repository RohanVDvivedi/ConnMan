#include<connman/server.h>
#include<connman/ssl_ctx_helper.h>

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

	//cgp = new_comm_address_tcp_ipv4(NULL, 7331);
	cgp = new_comm_address_tcp_ipv6(NULL, 7331);

	SSL_CTX* ssl_ctx = NULL;

	// comment below 2 lines to not use ssl
	ssl_lib_init();
	ssl_ctx = get_ssl_ctx_for_server("./cert.pem", "./key.pem");

	serve_using_stream_handlers(&cgp, NULL, connection_stream_handler, 10, ssl_ctx, 3000, &listen_fd); // timeout of 3 seconds

	if(ssl_ctx != NULL)
		destroy_ssl_ctx(ssl_ctx);

	return 0;
}

int process(char* buffer)
{
	printf("received : %s length %lu\n", buffer, strlen(buffer));

	if(strcmp(buffer, "exit\r\n") == 0 || strcmp(buffer, "exit\n") == 0 || strcmp(buffer, "exit") == 0)
	{
		strcpy(buffer, "xit\n");
		return -1;
	}
	else if(strcmp(buffer, "ping\r\n") == 0 || strcmp(buffer, "ping\n") == 0 || strcmp(buffer, "ping") == 0)
	{
		strcpy(buffer, "pong\n");
	}
	else if(strcmp(buffer, "pong\r\n") == 0 || strcmp(buffer, "pong\n") == 0 || strcmp(buffer, "pong") == 0)
	{
		strcpy(buffer, "ping\n");
	}

	return 0;
}

#define BUFFER_SIZE 64

void connection_stream_handler(stream* conn_strm, void* additional_params)
{
	printf("TCP Connection : %p\n", conn_strm);
	char buffer[BUFFER_SIZE + 1];

	int error;

	unsigned int buffreadlength = 0;
	unsigned int buffsentlength = 0;

	while(1)
	{
		buffreadlength = read_from_stream(conn_strm, buffer, BUFFER_SIZE, &error);
		if(error || buffreadlength == 0) // buffreadlength = 0, implies the connection is closed
			break;

		buffer[buffreadlength] = '\0';

		// process the buffer here
		int process_result = process(buffer);

		buffsentlength = strlen(buffer);
		write_to_stream(conn_strm, buffer, buffsentlength, &error);
		if(error)
			break;

		flush_all_from_stream(conn_strm, &error);
		if(error)
			break;

		if(process_result != 0)
		{
			break;
		}
	}
}