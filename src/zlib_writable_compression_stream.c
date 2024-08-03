#include<zlib_writable_compression_stream.h>

#include<zlib_stream_context.h>

#include<stdlib.h>

// must be lesser than SIZE_MAX and INT_MAX
#define OUT_CHUNK_SIZE 4096

static size_t write_to_stream_compressed(void* stream_context, const void* data, size_t data_size, int* error)
{
	// default error is no error
	(*error) = 0;

	zlib_stream_context* stream_context_p = stream_context;

	// intialize available in
	stream_context_p->zlib_context.next_in = (z_const Bytef *) data;
	stream_context_p->zlib_context.avail_in = data_size;

	size_t data_out_size = OUT_CHUNK_SIZE;
	char* data_out = malloc(sizeof(char) * data_out_size);

	// we failed to allocate an intermediate buffer
	if(data_out == NULL)
	{
		(*error) = ALLOCATION_FAILURE_IN_STREAM;
		return 0;
	}

	// loop while no error
	while(!(*error) && stream_context_p->zlib_context.avail_in > 0)
	{
		// initialize available out
		stream_context_p->zlib_context.next_out = (Bytef *) data_out;
		stream_context_p->zlib_context.avail_out = data_out_size;

		// compress
		int ret = deflate(&(stream_context_p->zlib_context), Z_NO_FLUSH);
		if(ret < 0)
		{
			(*error) = ret;
			break;
		}

		// if there are any bytes output from zlib then write then to underlying stream, if no bytes produced then quit this loop
		size_t bytes_to_write_to_underlying_strm = data_out_size - stream_context_p->zlib_context.avail_out;
		if(bytes_to_write_to_underlying_strm > 0)
		{
			int u_error = 0;
			write_to_stream(stream_context_p->underlying_strm, data_out, bytes_to_write_to_underlying_strm, &u_error);

			if(u_error)
				(*error) = UNDERLYING_STREAM_ERROR;
		}
	}

	// release data_out buffer
	free(data_out);

	// return number of bytes we consumed from data
	return data_size - stream_context_p->zlib_context.avail_in;
}

static void close_stream_context(void* stream_context, int* error)
{
	zlib_stream_context* stream_context_p = stream_context;

	// flush all bytes from zlib_context to the the underlying stream

	// intialize available in to hold 0 bytes
	stream_context_p->zlib_context.next_in = Z_NULL;
	stream_context_p->zlib_context.avail_in = 0;

	size_t data_out_size = OUT_CHUNK_SIZE;
	char* data_out = malloc(sizeof(char) * data_out_size);

	// we failed to allocate an intermediate buffer
	if(data_out == NULL)
	{
		(*error) = ALLOCATION_FAILURE_IN_STREAM;
		deflateEnd(&(stream_context_p->zlib_context));
		return ;
	}

	(*error) = 0;

	// loop while no error
	while(!(*error))
	{
		// initialize available out
		stream_context_p->zlib_context.next_out = (Bytef *) data_out;
		stream_context_p->zlib_context.avail_out = data_out_size;

		// compress
		int ret = deflate(&(stream_context_p->zlib_context), Z_FINISH);
		if(ret < 0)
		{
			(*error) = ret;
			break;
		}

		// if there are any bytes output from zlib then write then to underlying stream, if no bytes produced then quit this loop
		size_t bytes_to_write_to_underlying_strm = data_out_size - stream_context_p->zlib_context.avail_out;
		if(bytes_to_write_to_underlying_strm > 0)
		{
			int u_error = 0;
			write_to_stream(stream_context_p->underlying_strm, data_out, bytes_to_write_to_underlying_strm, &u_error);

			if(u_error)
				(*error) = UNDERLYING_STREAM_ERROR;
		}
		else
			break;
	}

	// release data_out buffer
	free(data_out);

	// actual closing of the zlib stream
	deflateEnd(&(stream_context_p->zlib_context));

	// now call flush on underlying stream
	if(!(*error))
		flush_all_from_stream(stream_context_p->underlying_strm, error);
}

static void destroy_stream_context(void* stream_context)
{
	free(stream_context);
}

static void post_flush_calling_underlying_stream_flush_zlib(void* stream_context, int* error)
{
	zlib_stream_context* stream_context_p = stream_context;
	flush_all_from_stream(stream_context_p->underlying_strm, error);
}

int initialize_stream_for_zlib_compression(stream* strm, stream* underlying_strm, int level)
{
	zlib_stream_context* stream_context = malloc(sizeof(zlib_stream_context));
	if(stream_context == NULL)
		return 0;

	stream_context->underlying_strm = underlying_strm;
	stream_context->zlib_context.zalloc = Z_NULL;
	stream_context->zlib_context.zfree = Z_NULL;
	stream_context->zlib_context.opaque = Z_NULL;

	if(Z_OK != deflateInit(&(stream_context->zlib_context), level))
	{
		free(stream_context);
		return 0;
	}

	if(!initialize_stream(strm, stream_context, NULL, write_to_stream_compressed, close_stream_context, destroy_stream_context, post_flush_calling_underlying_stream_flush_zlib, DEFAULT_MAX_UNFLUSHED_BYTES_COUNT))
	{
		int error = 0;
		close_stream_context(stream_context, &error);
		destroy_stream_context(stream_context);
		return 0;
	}

	return 1;
}

int initialize_stream_for_zlib_compression2(stream* strm, stream* underlying_strm, int level, int method, int windowBits, int memLevel, int strategy)
{
	zlib_stream_context* stream_context = malloc(sizeof(zlib_stream_context));
	if(stream_context == NULL)
		return 0;

	stream_context->underlying_strm = underlying_strm;
	stream_context->zlib_context.zalloc = Z_NULL;
	stream_context->zlib_context.zfree = Z_NULL;
	stream_context->zlib_context.opaque = Z_NULL;

	if(Z_OK != deflateInit2(&(stream_context->zlib_context), level, method, windowBits, memLevel, strategy))
	{
		free(stream_context);
		return 0;
	}

	if(!initialize_stream(strm, stream_context, NULL, write_to_stream_compressed, close_stream_context, destroy_stream_context, post_flush_calling_underlying_stream_flush_zlib, DEFAULT_MAX_UNFLUSHED_BYTES_COUNT))
	{
		int error = 0;
		close_stream_context(stream_context, &error);
		destroy_stream_context(stream_context);
		return 0;
	}

	return 1;
}