#ifndef STACKED_STREAM_H
#define STACKED_STREAM_H

#include<stream.h>
#include<stack.h>

/*
	This structure is not actually a stream, It is a container of streams
	It allows you to manage streams, when you are dealing with multiple streams each of which have an underlying streams
	stacked_streams will not own your streams that you stack on, you are responsible to allocate, free, initialize and deinitialize these streams
*/

typedef struct stacked_stream stacked_stream;
struct stacked_stream
{
	stack read_streams;
	stack write_streams;
};

// below two functions only initialize and deinitialize the stacked stream
// they do not allocate memory, free, intialize, deinitialize, or destroy your streams, you are solely responsible to do that
int initialize_stacked_stream(stacked_stream* sstrm);
void deinitialize_stacked_stream(stacked_stream* sstrm);

// the operate_on parameter can be one of the following in the below functions
#define NO_STREAMS		0b00
#define READ_STREAMS	0b01
#define WRITE_STREAMS	0b10
#define BOTH_STREAMS	0b11

// below functions work just like the cutlery stack works

// below functions only take READ_STREAMS or WRITE_STREAMS in their operate_on parameter
int is_empty_stacked_stream(stacked_stream* sstrm, int operate_on);
unsigned int get_stream_count_stacked_stream(stacked_stream* sstrm, int operate_on);
// push function may fail, if the strm is NULL or if the operate_on is READ_STREAMS while the stream is not readable,
// similarly it may also fail if the operate on is WRITE_STREAMS and the stream is not writable
int push_to_stacked_stream(stacked_stream* sstrm, stream* strm, int operate_on);
stream* get_top_of_stacked_stream(stacked_stream* sstrm, int operate_on);
stream* get_from_top_of_stacked_stream(stacked_stream* sstrm, unsigned int index, int operate_on);
int pop_from_stacked_stream(stacked_stream* sstrm, int operate_on);

// below functions work just like the corresponding functions in stream
// but they work only on top of their respective stream stacks
// i.e. read and unread work on the top stream of sstrm->read_streams
// while write and flush work on the top stream of sstrm->write_streams

size_t read_from_stacked_stream(stacked_stream* sstrm, void* data, size_t data_size, int* error);
void unread_from_stacked_stream(stacked_stream* sstrm, const void* data, size_t data_size, int* error);

size_t write_to_stacked_stream(stacked_stream* sstrm, const void* data, size_t data_size, int* error);
void flush_all_from_stacked_stream(stacked_stream* sstrm, int* error);

#include<stacked_stream_util.h>

#endif