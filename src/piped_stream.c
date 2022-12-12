#include<piped_stream.h>

#include<sync_pipe.h>

static unsigned int read_from_streamed_sync_pipe(void* stream_context, void* data, unsigned int data_size, int* error)
{
	return read_from_sync_pipe(((sync_pipe*)stream_context), data, data_size);
}

static unsigned int write_to_streamed_sync_pipe(void* stream_context, const void* data, unsigned int data_size, int* error)
{
	unsigned int bytes_written = write_to_sync_pipe(((sync_pipe*)stream_context), data, data_size);

	if(bytes_written == 0)
		*error = ERROR_PIPED_STREAM_CLOSED;

	return bytes_written;
}

static void close_stream_context_sync_pipe(void* stream_context, int* error)
{
	close_sync_pipe(stream_context);
}

static void destroy_stream_context_sync_pipe(void* stream_context)
{
	delete_sync_pipe(stream_context);
}

void initialize_piped_stream(stream* strm, unsigned int max_capacity)
{
	sync_pipe* stream_context = new_sync_pipe(max_capacity);
	initialize_stream(strm, stream_context, read_from_streamed_sync_pipe, write_to_streamed_sync_pipe, close_stream_context_sync_pipe, destroy_stream_context_sync_pipe);
}