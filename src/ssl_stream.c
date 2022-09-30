#include<ssl_stream.h>

static unsigned int read_from_ssl(void* ssl, void* data, unsigned int data_size, int* error)
{
	int ret = SSL_read(ssl, data, data_size);
	if(ret == -1)
	{
		*error = SSL_get_error(ssl, ret);
		return 0;
	}
	return ret;
}

static unsigned int write_to_ssl(void* ssl, const void* data, unsigned int data_size, int* error)
{
	int ret = SSL_write(ssl, data, data_size);
	if(ret == -1)
	{
		*error = SSL_get_error(ssl, ret);
		return 0;
	}
	return ret;
}

static void destroy_stream_context_ssl(void* stream_context)
{
	SSL_shutdown(stream_context);
	SSL_free(stream_context);
}

int initialize_stream_for_ssl_server(stream* strm, SSL_CTX* ctx, int fd)
{
	SSL* ssl = SSL_new(ctx);
	SSL_set_fd(ssl, fd);

	SSL_set_accept_state(ssl);
	if(SSL_accept(ssl) == -1)
	{
		SSL_free(ssl);
		return 0;
	}

	initialize_stream(strm, ssl, read_from_ssl, write_to_ssl, destroy_stream_context_ssl);

	return 1;
}

int initialize_stream_for_ssl_client(stream* strm, SSL_CTX* ctx, int fd)
{
	SSL* ssl = SSL_new(ctx);
	SSL_set_fd(ssl, fd);

	SSL_set_connect_state(ssl);
	if(SSL_connect(ssl) == -1)
	{
		SSL_free(ssl);
		return 0;
	}

	initialize_stream(strm, ssl, read_from_ssl, write_to_ssl, destroy_stream_context_ssl);

	return 1;
}