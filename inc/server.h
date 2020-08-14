#ifndef SERVER_H
#define SERVER_H

#include<connection_group.h>

#define INVALID_THREAD_COUNT 	59985923
#define INVALID_PROTOCOL     	59489213
#define INVALID_ADDRESS_FAMILY	59546876

/*
**	FOR TCP : 
		the handler is called once per connection
		in case of TCP server you get a cached thread pool executor of max thread_count threads

**  FOR UDP : 
		the handler is called for every thread that is created to send/receive message
		in case of UDP server you get a fixed thread pool executor of thread_count threads
*/

// here conn_fd, is the connection file discriptor, you can read and write to this discriptor, to communicate
// returns the file discriptor to the tcp or udp socket, on which the server is listening
int serve(connection_group* conn_grp_p, void* additional_params, void (*handler)(int conn_fd, void* additional_params), unsigned int thread_count, volatile int* listen_fd_p);

// stops the server that was started using the serve function
// it closes the file discriptor, hence the connection, here the parameter fd is the file discriptor of the listenning socket
int server_stop(int listen_fd);

#endif