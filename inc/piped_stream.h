#ifndef PIPED_STREAM_H
#define PIPED_STREAM_H

#include<stream.h>

// it internally uses a sync_dpipe to store the data

void initialize_piped_stream(stream* strm, unsigned int max_capacity);

#endif