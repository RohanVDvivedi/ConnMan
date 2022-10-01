#ifndef CLIENT_H
#define CLIENT_H

#include<comm_address.h>

#include<ssl_stream.h>
#include<file_descriptor_stream.h>

// returns file-discriptor to the socket, through which client connection has been made
// you must close the fd returned, after you are done with this client connection
// here, client_addr_p is optional, it can be used to connect to a server using a fixed port or ip
int make_connection(comm_address* server_addr_p, comm_address* client_addr_p);

// below function initializes a new stream connecting to server_addr_p
// if ssl_ctx is not NULL, then a ssl stream is returned
// it returns file_descriptor for success, else it returns -1 for an error
// you can close the connection by close_stream(strm), after you car done with the connection
// this function sould only be used with tcp connections
int make_connection_stream(stream* strm, comm_address* server_addr_p, comm_address* client_addr_p, SSL_CTX* ssl_ctx);

#endif