#include<stacked_stream.h>

// the (read_streams and write_streams) stacks in the stacked_stream are initialized with capacity equal to DEFAULT_STACKED_STREAM_STACKS_CAPACITY
#define DEFAULT_STACKED_STREAM_STACKS_CAPACITY 4

void initialize_stacked_stream(stacked_stream* sstrm)
{
	initialize_stack(&(sstrm->read_streams), DEFAULT_STACKED_STREAM_STACKS_CAPACITY);
	initialize_stack(&(sstrm->write_streams), DEFAULT_STACKED_STREAM_STACKS_CAPACITY);
}

void deinitialize_stacked_stream(stacked_stream* sstrm)
{
	deinitialize_stack(&(sstrm->read_streams));
	deinitialize_stack(&(sstrm->write_streams));
}

// all the bits that must be 0 for the operate_on parameter
#define NOT_OPERATE_ON_MASK (~BOTH_STREAMS)