#include<zlib_writable_compression_stream.h>

#include<stdlib.h>

#define OUT_CHUNK_SIZE 4096

static unsigned int write_to_stream_compressed(void* stream_context, const void* data, unsigned int data_size, int* error)
{
	zlib_stream_context* stream_context_p = stream_context;

	// intialize available in
	stream_context_p->zlib_context.next_in = (z_const Bytef *) data;
	stream_context_p->zlib_context.avail_in = data_size;

	unsigned int data_out_size = OUT_CHUNK_SIZE;
	char* data_out = malloc(sizeof(char) * data_out_size);

	(*error) = 0;

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
		unsigned int bytes_to_write_to_underlying_strm = data_out_size - stream_context_p->zlib_context.avail_out;
		if(bytes_to_write_to_underlying_strm > 0)
			write_to_stream(stream_context_p->underlying_strm, data_out, bytes_to_write_to_underlying_strm);
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

	unsigned int data_out_size = OUT_CHUNK_SIZE;
	char* data_out = malloc(sizeof(char) * data_out_size);

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
		unsigned int bytes_to_write_to_underlying_strm = data_out_size - stream_context_p->zlib_context.avail_out;
		if(bytes_to_write_to_underlying_strm > 0)
			write_to_stream(stream_context_p->underlying_strm, data_out, bytes_to_write_to_underlying_strm);
		else
			break;
	}

	// release data_out buffer
	free(data_out);

	// actual closing of the zlib stream
	deflateEnd(&(stream_context_p->zlib_context));
}

static void destroy_stream_context(void* stream_context)
{
	free(stream_context);
}

int initialize_stream_for_zlib_compression(stream* strm, stream* underlying_strm, int level)
{
	zlib_stream_context* stream_context = malloc(sizeof(zlib_stream_context));
	stream_context->underlying_strm = underlying_strm;
	stream_context->zlib_context.zalloc = Z_NULL;
	stream_context->zlib_context.zfree = Z_NULL;
	stream_context->zlib_context.opaque = Z_NULL;

	if(Z_OK != deflateInit(&(stream_context->zlib_context), level))
	{
		deflateEnd(&(stream_context->zlib_context));
		free(stream_context);
		return 0;
	}

	initialize_stream(strm, stream_context, NULL, write_to_stream_compressed, close_stream_context, destroy_stream_context);
	return 1;
}

int initialize_stream_for_zlib_compression2(stream* strm, stream* underlying_strm, int level, int method, int windowBits, int memLevel, int strategy)
{
	zlib_stream_context* stream_context = malloc(sizeof(zlib_stream_context));
	stream_context->underlying_strm = underlying_strm;
	stream_context->zlib_context.zalloc = Z_NULL;
	stream_context->zlib_context.zfree = Z_NULL;
	stream_context->zlib_context.opaque = Z_NULL;

	if(Z_OK != deflateInit2(&(stream_context->zlib_context), level, method, windowBits, memLevel, strategy))
	{
		deflateEnd(&(stream_context->zlib_context));
		free(stream_context);
		return 0;
	}

	initialize_stream(strm, stream_context, NULL, write_to_stream_compressed, close_stream_context, destroy_stream_context);
	return 1;
}