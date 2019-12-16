#ifndef TRANSACTION_CLINET_H
#define TRANSACTION_CLINET_H

#include<dstring.h>
#include<queue.h>
#include<hashmap.h>

#include<rwlock.h>

#include<client.h>

#include<job.h>

// the transaction_client is responsible for executing the transactions made on a connection with multiple threads one for each connection

// yopu have to first call this function, to use the transaction client
void intialize_transaction_client();

// this function will open an additional of a number of connections, for doing the transactions
void add_connections(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, int number_of_connections);

// the add_transaction_to function adds a transaction to be performed on a queue
// the transaction, can be performed by any of the connection, specified by tuple => (protocol, ip, port)
void add_transaction_to(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, void (*transaction)(int fd));

// this function will close all the connections to the specific
void close_all_connections_to(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT);

// to close all the connections and destroy all the resources occupied by the transaction_client
void destroy_transaction_client();

#endif