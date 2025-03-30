#include<connman/zlib_readable_decompression_stream.h>

#include<connman/zlib_stream_context.h>

#include<stdlib.h>

#include<cutlery/cutlery_math.h>

#define IN_CHUNK_SIZE min(MAX_UNREAD_BYTES_COUNT/2, INT_MAX)

static cy_uint read_from_stream_decompressed(void* stream_context, void* data, cy_uint data_size, int* error)
{
	// default error is no error
	(*error) = 0;

	zlib_stream_context* stream_context_p = stream_context;

	// initialize available out
	stream_context_p->zlib_context.next_out = (Bytef *) data;
	stream_context_p->zlib_context.avail_out = min(data_size, INT_MAX);

	cy_uint data_in_size = IN_CHUNK_SIZE;
	char* data_in = malloc(sizeof(char) * data_in_size);

	// we failed to allocate an intermediate buffer
	if(data_in == NULL)
	{
		(*error) = ALLOCATION_FAILURE_IN_STREAM;
		return 0;
	}

	while(!(*error))
	{
		// perform read from underlying stream
		int uerror = 0;
		cy_uint data_in_bytes_read = read_from_stream(stream_context_p->underlying_strm, data_in, data_in_size, &uerror);
		if(uerror)
		{
			(*error) = UNDERLYING_STREAM_ERROR;
			break;
		}

		// intialize available in
		stream_context_p->zlib_context.next_in = (z_const Bytef *) data_in;
		stream_context_p->zlib_context.avail_in = data_in_bytes_read;

		int flush = (data_in_bytes_read == 0) ? Z_FINISH : Z_NO_FLUSH;

		// decompress
		int ret = inflate(&(stream_context_p->zlib_context), flush);
		cy_uint data_in_bytes_consumed = data_in_bytes_read - stream_context_p->zlib_context.avail_in;
		if(ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR)
		{
			unread_from_stream(stream_context_p->underlying_strm, data_in + data_in_bytes_consumed, stream_context_p->zlib_context.avail_in, &uerror);
			(*error) = ret;
			break;
		}

		if(stream_context_p->zlib_context.avail_out == 0 || ret == Z_STREAM_END)
		{
			unread_from_stream(stream_context_p->underlying_strm, data_in + data_in_bytes_consumed, stream_context_p->zlib_context.avail_in, &uerror);
			if(uerror)
				(*error) = UNDERLYING_STREAM_ERROR;
			break;
		}

		if(flush == Z_FINISH)
			break;
	}

	free(data_in);

	return data_size - stream_context_p->zlib_context.avail_out;
}

static void close_stream_context(void* stream_context, int* error)
{
	zlib_stream_context* stream_context_p = stream_context;

	// actual closing of the zlib stream
	inflateEnd(&(stream_context_p->zlib_context));
}

static void destroy_stream_context(void* stream_context)
{
	free(stream_context);
}

int initialize_stream_for_zlib_decompression(stream* strm, stream* underlying_strm)
{
	zlib_stream_context* stream_context = malloc(sizeof(zlib_stream_context));
	if(stream_context == NULL)
		return 0;

	stream_context->underlying_strm = underlying_strm;
	stream_context->zlib_context.zalloc = Z_NULL;
	stream_context->zlib_context.zfree = Z_NULL;
	stream_context->zlib_context.opaque = Z_NULL;

	if(Z_OK != inflateInit(&(stream_context->zlib_context)))
	{
		free(stream_context);
		return 0;
	}

	if(!initialize_stream(strm, stream_context, read_from_stream_decompressed, NULL, close_stream_context, destroy_stream_context, NULL, 0))
	{
		int error = 0;
		close_stream_context(stream_context, &error);
		destroy_stream_context(stream_context);
		return 0;
	}

	return 1;
}

int initialize_stream_for_zlib_decompression2(stream* strm, stream* underlying_strm, int windowBits)
{
	zlib_stream_context* stream_context = malloc(sizeof(zlib_stream_context));
	if(stream_context == NULL)
		return 0;

	stream_context->underlying_strm = underlying_strm;
	stream_context->zlib_context.zalloc = Z_NULL;
	stream_context->zlib_context.zfree = Z_NULL;
	stream_context->zlib_context.opaque = Z_NULL;

	if(Z_OK != inflateInit2(&(stream_context->zlib_context), windowBits))
	{
		free(stream_context);
		return 0;
	}

	if(!initialize_stream(strm, stream_context, read_from_stream_decompressed, NULL, close_stream_context, destroy_stream_context, NULL, 0))
	{
		int error = 0;
		close_stream_context(stream_context, &error);
		destroy_stream_context(stream_context);
		return 0;
	}

	return 1;
}