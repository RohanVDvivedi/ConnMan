#ifndef TCP_SERVER_STREAM_HANDLER_H
#define TCP_SERVER_STREAM_HANDLER_H

#include<stream.h>

#include<openssl/ssl.h>

#define DEFAULT_BACKLOG_QUEUE_SIZE 10											// we do not ask the operating system to queue any more than 10 awaiting connections
#define DEFAULT_NO_CONNECTION_THREAD_DESTROY_TIMEOUT_IN_MICRO_SECONDS 3000 * 1000	// no new connection for 3 seconds and the waiting thread dies

int tcp_server_stream_handler(int listen_fd, void* additional_params, void (*stream_handler)(read_stream* rs, write_stream* ws, void* additional_params), unsigned int thread_count, SSL_CTX* ssl_ctx);

#endif