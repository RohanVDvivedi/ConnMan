#ifndef SERVER_H
#define SERVER_H

#include<connman/comm_address.h>

#include<cutlery/stream.h>

#include<openssl/ssl.h>

/*
**	FOR TCP : 
		the handler is called once per connection
		in case of TCP server you get a cached thread pool executor of max thread_count threads

**  FOR UDP : 
		the handler is called for every thread that is created to send/receive message
		in case of UDP server you get a fixed thread pool executor of thread_count threads
*/

// here conn_fd, is the connection file discriptor, you can read and write to this descriptor, to communicate
int serve_using_handlers(comm_address* server_addr_p, void* additional_params, void (*handler)(int conn_fd, void* additional_params), unsigned int thread_count, volatile int* listen_fd_p);

// serve using stream handlers instead of raw file descriptor handlers
// this only works for IP/TCP (if ssl_ctx in not NULL, then the connection will be IP/TCP/SSL), do not use it for IP/UDP
int serve_using_stream_handlers(comm_address* server_addr_p, void* additional_params, void (*stream_handler)(stream* strm, void* additional_params), unsigned int thread_count, SSL_CTX* ssl_ctx, volatile int* listen_fd_p);

// stops the server that was started using the serve_using_* functions
// it closes the listen file discriptor, here the parameter fd is the file discriptor of the listenning socket
int server_stop(int listen_fd);

#endif