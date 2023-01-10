#ifndef STREAM_UTIL_H
#define STREAM_UTIL_H

#include<stream.h>

#include<dstring.h>

#include<stdint.h>

unsigned int write_to_stream_formatted(stream* ws, const char* cstr_format, int* error, ...);

unsigned int read_uint64_from_stream(stream* rs, uint64_t* data, int* error);

unsigned int skip_whitespaces_from_stream(stream* rs, unsigned int max_whitespaces_to_skip, int* error);

// if return is non zero then an instance of str_to_skip was encountered
unsigned int skip_dstring_from_stream(stream* rs, const dstring* str_to_skip, int* error);

#endif