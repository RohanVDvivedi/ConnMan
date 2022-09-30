#include<ssl_stream.h>

#include<unistd.h>
#include<errno.h>

typedef struct ssl_stream_context ssl_stream_context;
struct ssl_stream_context
{
	int fd;
	SSL* ssl;
};

static unsigned int read_from_ssl(void* ssl_sc_vp, void* data, unsigned int data_size, int* error)
{
	ssl_stream_context* ssl_sc = ssl_sc_vp;
	int ret = SSL_read(ssl_sc->ssl, data, data_size);
	if(ret <= 0)
	{
		*error = SSL_get_error(ssl_sc->ssl, ret);
		return 0;
	}
	return ret;
}

static unsigned int write_to_ssl(void* ssl_sc_vp, const void* data, unsigned int data_size, int* error)
{
	ssl_stream_context* ssl_sc = ssl_sc_vp;

	int ret = SSL_write(ssl_sc->ssl, data, data_size);
	if(ret <= 0)
	{
		*error = SSL_get_error(ssl_sc->ssl, ret);
		return 0;
	}
	return ret;
}

static void close_stream_context_ssl(void* ssl_sc_vp, int* error)
{
	ssl_stream_context* ssl_sc = ssl_sc_vp;

	int ret = SSL_shutdown(ssl_sc->ssl);
	if(ret < 0)
		*error = SSL_get_error(ssl_sc->ssl, ret);

	ret = close(ssl_sc->fd);
	if(ret == -1)
		*error = errno;
}

static void destroy_stream_context_ssl(void* ssl_sc_vp)
{
	ssl_stream_context* ssl_sc = ssl_sc_vp;
	SSL_free(ssl_sc->ssl);
	free(ssl_sc);
}

int initialize_stream_for_ssl_server(stream* strm, SSL_CTX* ctx, int fd)
{
	ssl_stream_context* ssl_sc = malloc(sizeof(ssl_stream_context));

	ssl_sc->fd = fd;

	ssl_sc->ssl = SSL_new(ctx);
	SSL_set_fd(ssl_sc->ssl, fd);

	SSL_set_accept_state(ssl_sc->ssl);
	int ret = SSL_accept(ssl_sc->ssl);
	if(ret <= 0)
	{
		destroy_stream_context_ssl(ssl_sc);
		close(fd);
		return 0;
	}

	initialize_stream(strm, ssl_sc->ssl, read_from_ssl, write_to_ssl, close_stream_context_ssl, destroy_stream_context_ssl);

	return 1;
}

int initialize_stream_for_ssl_client(stream* strm, SSL_CTX* ctx, int fd)
{
	ssl_stream_context* ssl_sc = malloc(sizeof(ssl_stream_context));

	ssl_sc->fd = fd;

	ssl_sc->ssl = SSL_new(ctx);
	SSL_set_fd(ssl_sc->ssl, fd);

	SSL_set_connect_state(ssl_sc->ssl);
	int ret = SSL_connect(ssl_sc->ssl);
	if(ret <= 0)
	{
		destroy_stream_context_ssl(ssl_sc);
		close(fd);
		return 0;
	}

	initialize_stream(strm, ssl_sc, read_from_ssl, write_to_ssl, close_stream_context_ssl, destroy_stream_context_ssl);

	return 1;
}