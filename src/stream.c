#include<stream.h>

#include<stddef.h>
#include<string.h>

void initialize_stream(stream* strm, 
						void* stream_context,
						unsigned int (*read_from_stream_context)(void* stream_context, void* data, unsigned int data_size, int* error),
						unsigned int (*write_to_stream_context)(void* stream_context, const void* data, unsigned int data_size, int* error),
						void (*close_stream_context)(void* stream_context, int* error),
						void (*destroy_stream_context)(void* stream_context))
{
	strm->stream_context = stream_context;
	initialize_dpipe(&(strm->unread_data), 0);
	strm->read_from_stream_context = read_from_stream_context;
	strm->write_to_stream_context = write_to_stream_context;
	strm->close_stream_context = close_stream_context;
	strm->destroy_stream_context = destroy_stream_context;
	strm->error = 0;
}

int is_readable_stream(stream* strm)
{
	return strm->read_from_stream_context != NULL;
}

int is_writable_stream(stream* strm)
{
	return strm->write_to_stream_context != NULL;
}

int get_error_stream(stream* strm)
{
	return strm->error;
}

unsigned int min(unsigned int a, unsigned int b)
{
	return (a < b) ? a : b;
}

unsigned int read_from_stream(stream* strm, void* data, unsigned int data_size)
{
	if(strm->read_from_stream_context == NULL)
		return 0;

	if(data_size >= 128)
	{
		unsigned int bytes_read = read_from_dpipe(&(strm->unread_data), data, data_size, PARTIAL_ALLOWED);

		if(bytes_read < data_size)
			bytes_read += strm->read_from_stream_context(strm->stream_context, data + bytes_read, data_size - bytes_read, &(strm->error));

		return bytes_read;
	}
	else
	{
		unsigned int bytes_read = read_from_dpipe(&(strm->unread_data), data, data_size, PARTIAL_ALLOWED);

		if(bytes_read < data_size)
		{
			char data_cache_read[1024];
			unsigned int data_cache_read_capacity = 1024;
			unsigned int data_cache_read_size = strm->read_from_stream_context(strm->stream_context, data_cache_read, data_cache_read_capacity, &(strm->error));

			unsigned int cache_bytes_to_move_to_output_buffer = min(data_size - bytes_read, data_cache_read_size);

			memmove(data + bytes_read, data_cache_read, cache_bytes_to_move_to_output_buffer);
			bytes_read += cache_bytes_to_move_to_output_buffer;

			unread_from_stream(strm, data_cache_read + cache_bytes_to_move_to_output_buffer, data_cache_read_size - cache_bytes_to_move_to_output_buffer);
		}

		return bytes_read;
	}
}

int unread_from_stream(stream* strm, const void* data, unsigned int data_size)
{
	if(strm->read_from_stream_context == NULL)
		return 0;

	unsigned int bytes_unread = unread_to_dpipe(&(strm->unread_data), data, data_size, ALL_OR_NONE);

	if(bytes_unread == 0)
	{
		resize_dpipe(&(strm->unread_data), get_capacity_dpipe(&(strm->unread_data)) + data_size + 1024);
		bytes_unread = unread_to_dpipe(&(strm->unread_data), data, data_size, ALL_OR_NONE);
	}

	return bytes_unread == data_size;
}

unsigned int write_to_stream(stream* strm, const void* data, unsigned int data_size)
{
	if(strm->write_to_stream_context == NULL)
		return 0;

	unsigned int bytes_written = 0;
	while(bytes_written < data_size)
	{
		bytes_written += strm->write_to_stream_context(strm->stream_context, data + bytes_written, data_size - bytes_written, &(strm->error));

		if(strm->error)
			break;
	}
	return bytes_written;
}

void close_stream(stream* strm)
{
	strm->close_stream_context(strm->stream_context, &strm->error);
}

void deinitialize_stream(stream* strm)
{
	deinitialize_dpipe(&(strm->unread_data));
	strm->destroy_stream_context(strm->stream_context);
	*strm = (stream){};
}