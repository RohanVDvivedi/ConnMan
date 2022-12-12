#include<writable_dstring_stream.h>

#include<dstring.h>

#include<stdlib.h>

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
	deinit_dstring((dstring*)stream_context);
	free(stream_context);
}

void initialize_writable_dstring_stream(stream* strm)
{
	dstring* stream_context = malloc(sizeof(dstring));
	init_empty_dstring(stream_context, 0);
	initialize_stream(strm, stream_context, NULL, write_to_writable_dstring_stream, close_stream_context_writable_dstring_stream, destroy_stream_context_writable_dstring_stream);
}