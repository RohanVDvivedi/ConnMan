#ifndef SSL_STREAM_H
#define SSL_STREAM_H

#include<cutlery/stream.h>

#include<openssl/ssl.h>

// 0 indicates a failure, to SSL_accept a connection
int initialize_stream_for_ssl_server(stream* strm, SSL_CTX* ctx, int fd);
int initialize_stream_for_ssl_server_over_stream(stream* strm, SSL_CTX* ctx, stream* underlying_strm);

// 0 indicates a failure, to SSL_connect a connection
int initialize_stream_for_ssl_client(stream* strm, SSL_CTX* ctx, const char* hostname, int fd);
int initialize_stream_for_ssl_client_over_stream(stream* strm, SSL_CTX* ctx, const char* hostname, stream* underlying_strm);

#endif