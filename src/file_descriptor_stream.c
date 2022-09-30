#include<file_descriptor_stream.h>

#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

static unsigned int read_from_fd(void* stream_context, void* data, unsigned int data_size, int* error)
{
	ssize_t ret = read(*((int*)stream_context), data, data_size);
	if(ret == -1)
	{
		*error = errno;
		return 0;
	}
	return ret;
}

static unsigned int write_to_fd(void* stream_context, const void* data, unsigned int data_size, int* error)
{
	ssize_t ret = write(*((int*)stream_context), data, data_size);
	if(ret == -1)
	{
		*error = errno;
		return 0;
	}
	return ret;
}

static void destroy_stream_context_fd(void* stream_context)
{
	free(stream_context);
}

void initialize_stream_for_fd(stream* strm, int fd)
{
	int* stream_context = malloc(sizeof(int));
	*stream_context = fd;
	initialize_stream(strm, stream_context, read_from_fd, write_to_fd, destroy_stream_context_fd);
}