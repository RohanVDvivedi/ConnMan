#include<file_descriptor_stream.h>

#include<stdlib.h>
#include<unistd.h>

static unsigned int read_from_stream(void* stream_context, void* data, unsigned int data_size)
{
	ssize_t bytes_read = read(*((int*)stream_context), data, data_size);

	if(bytes_read == -1)
		return 0;

	return bytes_read;
}

static unsigned int write_to_stream(void* stream_context, const void* data, unsigned int data_size)
{
	unsigned int bytes_written = 0;
	while(data_size)
	{
		ssize_t ret = write(*((int*)stream_context), data + bytes_written, data_size - bytes_written);

		if(ret == -1)
			break;

		bytes_written += ret;
	}
	return bytes_written;
}

static int close_stream(void* stream_context)
{
	return close(*((int*)stream_context));
}

void initialize_read_stream_for_fd(read_stream* rs, int fd)
{
	rs->stream_context = malloc(sizeof(int));
	*((int*)rs->stream_context) = fd;
	rs->read_from_stream = read_from_stream;
	rs->close_stream = close_stream;
}

void initialize_write_stream_for_fd(write_stream* ws, int fd)
{
	ws->stream_context = malloc(sizeof(int));
	*((int*)ws->stream_context) = fd;
	ws->write_to_stream = write_to_stream;
	ws->close_stream = close_stream;
}

void deinitialize_read_stream_for_fd(read_stream* rs)
{
	close_stream(rs->stream_context);
	free(rs->stream_context);
	(*rs) = (read_stream){};
}

void deinitialize_write_stream_for_fd(write_stream* ws)
{
	close_stream(ws->stream_context);
	free(ws->stream_context);
	(*ws) = (write_stream){};
}