#ifndef CLIENT_H
#define CLIENT_H

#include<connection_group.h>

int connect_to(connection_group* conn_grp_p, void (*handler)(int fd));

// the below are the sub functions used to build the connect_to functions

// returns file-discriptor to the socket, through which client connection has been made
int make_connection(connection_group* conn_grp_p);

#endif