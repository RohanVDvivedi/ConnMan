#include<stream.h>

#include<stddef.h>

void initialize_stream(stream* strm, void* stream_context, unsigned int (*read_from_stream_context)(void* stream_context, void* data, unsigned int data_size, int* error), unsigned int (*write_to_stream_context)(void* stream_context, const void* data, unsigned int data_size, int* error), void (*destroy_stream_context)(void* stream_context))
{
	strm->stream_context = stream_context;
	initialize_dpipe(&(strm->unread_data), 0);
	strm->read_from_stream_context = read_from_stream_context;
	strm->write_to_stream_context = write_to_stream_context;
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

unsigned int read_from_stream(stream* strm, void* data, unsigned int data_size)
{
	if(strm->read_from_stream_context == NULL)
		return 0;

	unsigned int bytes_read = read_from_dpipe(&(strm->unread_data), data, data_size, PARTIAL_ALLOWED);

	if(bytes_read < data_size)
		bytes_read += strm->read_from_stream_context(strm->stream_context, data + bytes_read, data_size - bytes_read, &(strm->error));

	return bytes_read;
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

void deinitialize_stream(stream* strm)
{
	deinitialize_dpipe(&(strm->unread_data));
	strm->destroy_stream_context(strm->stream_context);
	*strm = (stream){};
}