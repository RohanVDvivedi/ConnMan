#ifndef SSL_CTX_HELPER_H
#define SSL_CTX_HELPER_H

#include<openssl/ssl.h>

void ssl_lib_init();

SSL_CTX* get_ssl_ctx_for_server(char* cert_file, char* key_file);

SSL_CTX* get_ssl_ctx_for_client(char* cert_file, char* key_file);

void destroy_ssl_ctx(SSL_CTX* ctx);

#endif