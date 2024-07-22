#ifndef SSL_STREAM_H
#define SSL_STREAM_H

#include<stream.h>

#include<openssl/ssl.h>

// 0 indicates a failure, to SSL_accept a connection
int initialize_stream_for_ssl_server(stream* strm, SSL_CTX* ctx, int fd);

// 0 indicates a failure, to SSL_connect a connection
int initialize_stream_for_ssl_client(stream* strm, SSL_CTX* ctx, const char* hostname, int fd);

#endif