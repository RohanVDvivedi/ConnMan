#ifndef ZLIB_READABLE_DECOMPRESSION_STREAM_H
#define ZLIB_READABLE_DECOMPRESSION_STREAM_H

#include<stream.h>
#include<zlib.h>

#include<zlib_stream_context.h>

// 0 indicates failute, 1 implies success
int initialize_stream_for_zlib_decompression(stream* strm, stream* underlying_strm);

int initialize_stream_for_zlib_decompression2(stream* strm, stream* underlying_strm, int windowBits);

#endif