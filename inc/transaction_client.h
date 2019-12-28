#ifndef TRANSACTION_CLINET_H
#define TRANSACTION_CLINET_H

#include<connection_group.h>
#include<client.h>

// the add_transaction_to function adds a transaction to be performed on a queue
// the transaction, can be performed by any of the connection, specified by tuple => (protocol, ip, port)
void add_transaction_to_connection_group(connection_group* conn_grp_p, int (*transaction)(int fd, void* additional_params), void* additional_params);

#endif