#ifndef STREAM_UTIL_H
#define STREAM_UTIL_H

#include<stream_util.h>

#include<stream.h>

unsigned int write_uint64_to_stream(stream* ws, uint64_t u);

unsigned int write_int64_to_stream(stream* ws, int64_t i);

unsigned int write_dstring_to_stream(stream* ws, const dstring* str);

unsigned int write_cstring_to_stream(stream* ws, const char* str);

#endif