#include<client_set.h>

client_set* new_client_set(comm_address* server_addr_p, SSL_CTX* ssl_ctx, unsigned int max_clients);

unsigned int get_max_clients(client_set* cls);

void reset_max_clients(client_set* cls, unsigned int max_clients);

stream* reserve_client(client_set* cls);

void return_client(client_set* cls, stream* strm);

void shutdown_client_set(client_set* cls);

void delete_client_set(client_set* cls);