#ifndef FILE_DESCRIPTOR_STREAM
#define FILE_DESCRIPTOR_STREAM

#include<stream.h>

void initialize_read_stream_for_fd(read_stream* rs, int fd);

void initialize_write_stream_for_fd(write_stream* ws, int fd);

void deinitialize_read_stream_for_fd(read_stream* rs);

void deinitialize_write_stream_for_fd(write_stream* ws);

#endif