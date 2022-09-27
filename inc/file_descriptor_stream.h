#ifndef FILE_DESCRIPTOR_STREAM
#define FILE_DESCRIPTOR_STREAM

#include<stream.h>

void initialize_streams_for_fd(read_stream* rs, write_stream* ws, int fd);

void deinitialize_fd_streams(read_stream* rs, write_stream* ws);

#endif