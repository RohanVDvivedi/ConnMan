#include<writable_dstring_stream.h>

#include<stddef.h>

static size_t write_to_writable_dstring_stream(void* stream_context, const void* data, size_t data_size, int* error)
{
	if(concatenate_dstring((dstring*)stream_context, &get_dstring_pointing_to(data, data_size)))
		return data_size;
	(*error) = UNDERLYING_STREAM_ERROR;
	return 0;
}

static void close_stream_context_writable_dstring_stream(void* stream_context, int* error)
{
	// close is a NOP
}

static void destroy_stream_context_writable_dstring_stream(void* stream_context)
{
	// destroy is a NOP
	// you need to manually destroy the dstring
}

int initialize_writable_dstring_stream(stream* strm, dstring* write_to)
{
	if(write_to == NULL || get_dstring_type(write_to) == POINT_DSTR)
		return 0;

	if(!initialize_stream(strm, write_to, NULL, write_to_writable_dstring_stream, close_stream_context_writable_dstring_stream, destroy_stream_context_writable_dstring_stream, NULL, DEFAULT_MAX_UNFLUSHED_BYTES_COUNT))
	{
		int error = 0;
		close_stream_context_writable_dstring_stream(write_to, &error);
		destroy_stream_context_writable_dstring_stream(write_to);
		return 0;
	}

	return 1;
}