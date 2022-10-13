#include<ssl_ctx_helper.h>

void ssl_lib_init()
{
	SSL_library_init();
	SSL_load_error_strings();
}

SSL_CTX* get_ssl_ctx_for_server(char* cert_file, char* key_file)
{
	SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_server_method());

	if(SSL_CTX_use_certificate_file(ctx, cert_file, SSL_FILETYPE_PEM) <= 0)
		goto ERR_EXIT;
	
	if(SSL_CTX_use_PrivateKey_file(ctx, key_file, SSL_FILETYPE_PEM) <= 0)
		goto ERR_EXIT;
	
	if(!SSL_CTX_check_private_key(ctx))
		goto ERR_EXIT;

	return ctx;

	ERR_EXIT : ;
	SSL_CTX_free(ctx);
	return NULL;
}

SSL_CTX* get_ssl_ctx_for_client()
{
	SSL_CTX* ctx = SSL_CTX_new(TLSv1_2_client_method());

	return ctx;
}

void destroy_ssl_ctx(SSL_CTX* ctx)
{
	SSL_CTX_free(ctx);
}