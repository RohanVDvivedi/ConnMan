#ifndef STREAM_UTIL_H
#define STREAM_UTIL_H

#include<stream.h>

#include<dstring.h>

#include<stdint.h>

unsigned int write_to_stream_formatted(stream* ws, int* error, const char* cstr_format, ...);

// supported radix for below functions are only
#define BINARY 		2
#define OCTAL 		8
#define DECIMAL 	10
#define HEXADECIMAL 16

unsigned int read_uint64_from_stream(stream* rs, unsigned int radix, uint64_t* data, int* error);

unsigned int skip_whitespaces_from_stream(stream* rs, unsigned int max_whitespaces_to_skip, int* error);

// if return is non zero then an instance of str_to_skip was encountered
unsigned int skip_dstring_from_stream(stream* rs, const dstring* str_to_skip, int* error);

// bytes will be read until the until_str dstring is encountered in the stream
// all bytes read from the stream will be retunred in the return dstring
// on success (if until dstring is encountered) the returned dstring will also contain until_str as its suffix
// on failure (if max_bytes_to_read is encountered OR the stream is closed) then an empty dstring will be returned
// a direct failure results if the max_bytes_to_read is lesser than the size of until_str
// in any case, no more than max_bytes_to_read bytes will be read
// user is expected to call deinit_dstring on the returned dstring
// NOTE: suffix_prefix_match_lengths is the one computed by cutlery
dstring read_until_dstring_from_stream(stream* rs, const dstring* until_str, const unsigned int* prefix_suffix_match_lengths_for_until_str, unsigned int max_bytes_to_read, int* error);

// this function returns a dstring from stream rs, until any of an end char is encountered
// unless max_bytes_to_read are encountered first, then an empty streing is returned
// last_byte will be set to the last byte read, and 256 for end of stream
dstring read_until_any_end_chars_from_stream(stream* rs, int (*is_end_char)(int is_end_of_stream, char c, const void* cntxt), const void* cntxt, int* last_byte, unsigned int max_bytes_to_read, int* error);

// below are utility functions to directly use dstrings for writing and unreading bytes to stream
int unread_dstring_from_stream(stream* rs, const dstring* str);
unsigned int write_dstring_to_stream(stream* ws, const dstring* str, int* error);

#endif