#include<zlib_writable_compression_stream.h>

#include<stdlib.h>

static unsigned int write_to_stream_compressed(void* stream_context, const void* data, unsigned int data_size, int* error)
{
	zlib_stream_context* stream_context_p = stream_context;

	// compress and then write to underlying stream
}

static void close_stream_context(void* stream_context, int* error)
{
	zlib_stream_context* stream_context_p = stream_context;

	// flush everything to the underlying stream

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

	if(Z_OK != deflateInit2(&(stream_context->zlib_context), level, method, windowBits, memLevel, strategy))
	{
		deflateEnd(&(stream_context->zlib_context));
		free(stream_context);
		return 0;
	}

	initialize_stream(strm, stream_context, NULL, write_to_stream_compressed, close_stream_context, destroy_stream_context);
	return 1;
}