#ifndef ZLIB_STREAM_CONTEXT_H
#define ZLIB_STREAM_CONTEXT_H

typedef struct zlib_stream_context zlib_stream_context;
struct zlib_stream_context
{
	stream* underlying_strm;
	z_stream zlib_context;
};

#endif