#include<writable_dstring_stream.h>

#include<stddef.h>

static unsigned int write_to_writable_dstring_stream(void* stream_context, const void* data, unsigned int data_size, int* error)
{
	const dstring append_str = get_literal_dstring(data, data_size);
	if(concatenate_dstring((dstring*)stream_context, &append_str))
		return data_size;
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
	initialize_stream(strm, write_to, NULL, write_to_writable_dstring_stream, close_stream_context_writable_dstring_stream, destroy_stream_context_writable_dstring_stream);
	return 1;
}