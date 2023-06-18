#include<ssl_stream.h>

#include<unistd.h>
#include<errno.h>

#include<cutlery_math.h>

static size_t read_from_ssl(void* ssl_sc_vp, void* data, size_t data_size, int* error)
{
	SSL* ssl = ssl_sc_vp;

	int ret = SSL_read(ssl, data, min(data_size, INT_MAX));
	if(ret <= 0)
	{
		*error = SSL_get_error(ssl, ret);
		return 0;
	}
	return ret;
}

static size_t write_to_ssl(void* ssl_sc_vp, const void* data, size_t data_size, int* error)
{
	SSL* ssl = ssl_sc_vp;

	int ret = SSL_write(ssl, data, min(data_size, INT_MAX));
	if(ret <= 0)
	{
		*error = SSL_get_error(ssl, ret);
		return 0;
	}
	return ret;
}

static void close_stream_context_ssl(void* ssl_sc_vp, int* error)
{
	SSL* ssl = ssl_sc_vp;
	int fd = SSL_get_fd(ssl);

	int ret = SSL_shutdown(ssl);
	if(ret < 0)
		*error = SSL_get_error(ssl, ret);

	ret = close(fd);
	if(ret == -1)
		*error = errno;
}

static void destroy_stream_context_ssl(void* ssl_sc_vp)
{
	SSL* ssl = ssl_sc_vp;
	
	SSL_free(ssl);
}

int initialize_stream_for_ssl_server(stream* strm, SSL_CTX* ctx, int fd)
{
	SSL* ssl = SSL_new(ctx);
	SSL_set_fd(ssl, fd);

	SSL_set_accept_state(ssl);
	int ret = SSL_accept(ssl);
	if(ret <= 0)
	{
		SSL_free(ssl);
		return 0;
	}

	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

	initialize_stream(strm, ssl, read_from_ssl, write_to_ssl, close_stream_context_ssl, destroy_stream_context_ssl, NULL, DEFAULT_MAX_UNFLUSHED_BYTES_COUNT);

	return 1;
}

int initialize_stream_for_ssl_client(stream* strm, SSL_CTX* ctx, int fd)
{
	SSL* ssl = SSL_new(ctx);
	SSL_set_fd(ssl, fd);

	SSL_set_connect_state(ssl);
	int ret = SSL_connect(ssl);
	if(ret <= 0)
	{
		SSL_free(ssl);
		return 0;
	}

	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

	initialize_stream(strm, ssl, read_from_ssl, write_to_ssl, close_stream_context_ssl, destroy_stream_context_ssl, NULL, DEFAULT_MAX_UNFLUSHED_BYTES_COUNT);

	return 1;
}