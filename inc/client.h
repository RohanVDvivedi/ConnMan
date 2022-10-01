#ifndef CLIENT_H
#define CLIENT_H

#include<comm_address.h>

// returns file-discriptor to the socket, through which client connection has been made
// you must close the fd returned, after you are done with this client connection
// here, client_addr_p is optional, it can be used to connect to a server using a fixed port or ip
int client_connect(comm_address* server_addr_p, comm_address* client_addr_p);

#endif