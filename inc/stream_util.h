#ifndef STREAM_UTIL_H
#define STREAM_UTIL_H

#include<stream_util.h>

#include<stream.h>

unsigned int write_to_stream_formatted(stream* ws, const char* cstr_format, int* error, ...);

unsigned int read_uint64_from_stream(stream* rs, uint64_t* data, int* error);

#endif