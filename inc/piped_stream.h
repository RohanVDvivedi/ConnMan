#ifndef PIPED_STREAM_H
#define PIPED_STREAM_H

#include<stream.h>

// it internally uses a sync_dpipe to store the data

#define ERROR_PIPED_STREAM_CLOSED -1

void initialize_piped_stream(stream* strm, unsigned int max_capacity);

#endif