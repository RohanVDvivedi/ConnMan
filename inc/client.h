#ifndef CLIENT_H
#define CLIENT_H

#include<comm_address.h>

// returns file-discriptor to the socket, through which client connection has been made
// you must close the fd returned, after you are done with this client connection
int client_connect(comm_address* server_addr_p, uint16_t client_port);

#endif