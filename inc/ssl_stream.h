#ifndef SSL_STREAM_H
#define SSL_STREAM_H

#include<stream.h>

#include<openssl/ssl.h>

// 0 indicates a failire, to accept a connection
int initialize_streams_for_ssl(read_stream* rs, write_stream* ws, SSL_CTX* ctx, int conn_fd);

void deinitialize_streams_for_ssl(read_stream* rs, write_stream* ws);

#endif