#include<connman/client.h>
#include<connman/ssl_ctx_helper.h>

#include<string.h>
#include<stdio.h>
#include<unistd.h>

void connection_stream_handler(stream* strm);

int main()
{
	comm_address cgp;

	//cgp = new_comm_address_tcp_ipv4("127.0.0.1", 7331);
	cgp = new_comm_address_tcp_ipv6("::1", 7331);

	SSL_CTX* ssl_ctx = NULL;

	// comment below 2 lines to not use ssl
	ssl_lib_init();
	ssl_ctx = get_ssl_ctx_for_client("./cert.pem", "./key.pem");

	stream strm;
	int res = make_connection_stream(&strm, &cgp, "", NULL, ssl_ctx, 3000); // timeout of 3 seconds

	if(res < 0)
		return -1;

	connection_stream_handler(&strm);

	int close_error;
	close_stream(&strm, &close_error);

	deinitialize_stream(&strm);

	if(ssl_ctx != NULL)
		destroy_ssl_ctx(ssl_ctx);

	return 0;
}

#define BUFFER_SIZE 64

void connection_stream_handler(stream* strm)
{
	char buffer[BUFFER_SIZE + 1];
	int error;

	while(1)
	{
		printf("self : ");
		scanf("%s", buffer);

		write_to_stream(strm, buffer, strlen(buffer), &error);
		if(error)
			break;

		flush_all_from_stream(strm, &error);
		if(error)
			break;

		int buffreadlength = read_from_stream(strm, buffer, BUFFER_SIZE, &error);
		if(error || buffreadlength == 0) // buffreadlength = 0, implies the connection is closed
			break;
		buffer[buffreadlength] = '\0';

		printf("server : %s\n", buffer);
	}
}