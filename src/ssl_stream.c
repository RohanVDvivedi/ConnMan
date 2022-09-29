#include<ssl_stream.h>

static unsigned int read_from_stream(void* ssl_stream_context, void* data, unsigned int data_size)
{
	int bytes_read = SSL_read(ssl_stream_context, data, data_size);

	if(bytes_read < 0)
		return 0;

	return bytes_read;
}

static unsigned int write_to_stream(void* ssl_stream_context, const void* data, unsigned int data_size)
{
	unsigned int bytes_written = 0;
	while(bytes_written < data_size)
	{
		int ret = SSL_write(ssl_stream_context, data + bytes_written, data_size - bytes_written);

		if(ret < 0)
			break;

		bytes_written += ret;
	}
	return bytes_written;
}

int initialize_streams_for_ssl_server(read_stream* rs, write_stream* ws, SSL_CTX* ctx, int conn_fd)
{
	SSL* ssl = SSL_new(ctx);
	SSL_set_fd(ssl, conn_fd);

	if(SSL_accept(ssl) == -1)
	{
		SSL_free(ssl);
		return 0;
	}

	rs->stream_context = ssl;
	rs->read_from_stream = read_from_stream;

	ws->stream_context = ssl;
	ws->write_to_stream = write_to_stream;

	return 1;
}

int initialize_streams_for_ssl_client(read_stream* rs, write_stream* ws, SSL_CTX* ctx, int conn_fd)
{
	SSL* ssl = SSL_new(ctx);
	SSL_set_fd(ssl, conn_fd);

	if(SSL_connect(ssl) == -1)
	{
		SSL_free(ssl);
		return 0;
	}

	rs->stream_context = ssl;
	rs->read_from_stream = read_from_stream;

	ws->stream_context = ssl;
	ws->write_to_stream = write_to_stream;

	return 1;
}

void deinitialize_streams_for_ssl(read_stream* rs, write_stream* ws)
{
	SSL_shutdown(rs->stream_context);
	SSL_free(rs->stream_context);
	(*rs) = (read_stream){};
	(*ws) = (write_stream){};
}