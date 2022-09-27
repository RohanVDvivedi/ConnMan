#include<file_descriptor_stream.h>

#include<stdlib.h>
#include<errno.h>

static unsigned int read_from_stream(void* stream_context, void* data, unsigned int data_size)
{
	ssize_t bytes_read = recv(*((int*)stream_context), data, data_size, 0);

	if(bytes_read == -1)
		return 0;

	return bytes_read;
}

static unsigned int write_to_stream(void* stream_context, const void* data, unsigned int data_size)
{
	unsigned int bytes_written = 0;
	while(data_size)
	{
		ssize_t ret = send(*((int*)stream_context), data + bytes_sent, data_size - bytes_sent, 0);

		if(ret == -1)
			return;

		bytes_written += ret;
	}
}

static int close_stream(void* stream_context)
{
	return close(*((int*)stream_context));
}

void initialize_streams_for_fd(read_stream* rs, write_stream* ws, int fd)
{
	rs->stream_context = malloc(sizeof(int));
	*((int*)rs->stream_context) = fd;
	ws->stream_context = malloc(sizeof(int));
	*((int*)ws->stream_context) = fd;

	rs->read_from_stream = read_from_stream;
	ws->write_to_stream = write_to_stream;

	rs->close_stream = close_stream;
	ws->close_stream = close_stream;
}

void deinitialize_fd_streams(read_stream* rs, write_stream* ws)
{
	close_stream(rs->stream_context);

	free(rs->stream_context);
	(*rs) = (read_stream){};

	free(ws->stream_context);
	(*ws) = (write_stream){};
}