#include<piped_stream.h>

#include<sync_pipe.h>

static cy_uint read_from_streamed_sync_pipe(void* stream_context, void* data, cy_uint data_size, int* error)
{
	return read_from_sync_pipe(((sync_pipe*)stream_context), data, data_size);
}

static cy_uint write_to_streamed_sync_pipe(void* stream_context, const void* data, cy_uint data_size, int* error)
{
	cy_uint bytes_written = write_to_sync_pipe(((sync_pipe*)stream_context), data, data_size);

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

int initialize_piped_stream(stream* strm, cy_uint max_capacity)
{
	sync_pipe* stream_context = new_sync_pipe(max_capacity);
	if(stream_context == NULL)
		return 0;
	if(!initialize_stream(strm, stream_context, read_from_streamed_sync_pipe, write_to_streamed_sync_pipe, close_stream_context_sync_pipe, destroy_stream_context_sync_pipe, NULL, DEFAULT_MAX_UNFLUSHED_BYTES_COUNT))
	{
		int error = 0;
		close_stream_context_sync_pipe(stream_context, &error);
		destroy_stream_context_sync_pipe(stream_context);
		return 0;
	}
	return 1;
}