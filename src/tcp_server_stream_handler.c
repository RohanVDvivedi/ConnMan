#include<tcp_server_stream_handler.h>

#include<stream_handler_wrapper_params.h>

int tcp_server_stream_handler(int listen_fd, void* additional_params, void (*stream_handler)(read_stream* rs, write_stream* ws, void* additional_params), unsigned int thread_count, SSL_CTX* ssl_ctx)
{
	return -1;
}