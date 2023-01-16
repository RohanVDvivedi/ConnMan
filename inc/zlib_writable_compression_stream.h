#ifndef ZLIB_WRITABLE_COMPRESSION_STREAM_H
#define ZLIB_WRITABLE_COMPRESSION_STREAM_H

#include<stream.h>
#include<zlib.h>

typedef struct zlib_stream_context zlib_stream_context;
struct zlib_stream_context
{
	stream* underlying_strm;
	z_stream zlib_context;
};

// 0 indicates failute, 1 implies success
int initialize_stream_for_zlib_compression(stream* strm, stream* underlying_strm, int level);

int initialize_stream_for_zlib_compression2(stream* strm, stream* underlying_strm, int level, int method, int windowBits, int memLevel, int strategy);

#endif