#ifndef STACKED_STREAM_UTIL_H
#define STACKED_STREAM_UTIL_H

#include<connman/stacked_stream.h>

void close_deinitialize_free_all_from_stacked_stream(stacked_stream* sstrm, int operate_on);

void close_deinitialize_free_all_from_WRITER_stacked_stream(stacked_stream* sstrm);

void close_deinitialize_free_all_from_READER_stacked_stream(stacked_stream* sstrm);

#endif