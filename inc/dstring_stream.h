#ifndef DSTRING_STREAM_H
#define DSTRING_STREAM_H

#include<stream.h>

// it internally uses a unbounded_sync_dpipe to store the dstring

void initialize_dstring_stream(stream* strm);

#endif