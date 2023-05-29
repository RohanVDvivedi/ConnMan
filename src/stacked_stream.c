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

int is_empty_stacked_stream(stacked_stream* sstrm, int operate_on)
{
	switch(operate_on)
	{
		case READ_STREAMS :
			return is_empty_stack(&(sstrm->read_streams));
		case WRITE_STREAMS :
			return is_empty_stack(&(sstrm->write_streams));
		default :
			return 1;
	}
}

unsigned int get_stream_count_stacked_stream(stacked_stream* sstrm, int operate_on)
{
	switch(operate_on)
	{
		case READ_STREAMS :
			return get_element_count_stack(&(sstrm->read_streams));
		case WRITE_STREAMS :
			return get_element_count_stack(&(sstrm->write_streams));
		default :
			return 0;
	}
}

int push_to_stacked_stream(stacked_stream* sstrm, stream* strm, int operate_on)
{
	if(strm == NULL)
		return 0;

	switch(operate_on)
	{
		case READ_STREAMS :
		{
			if(!is_readable_stream(strm))
				return 0;
			if(is_full_stack(&sstrm->read_streams) && !expand_stack(&sstrm->read_streams))
				return 0;
			return push_to_stack(&(sstrm->read_streams), strm);
		}
		case WRITE_STREAMS :
		{
			if(!is_writable_stream(strm))
				return 0;
			if(is_full_stack(&sstrm->write_streams) && !expand_stack(&sstrm->write_streams))
				return 0;
			return push_to_stack(&(sstrm->write_streams), strm);
		}
		case BOTH_STREAMS :
		{
			if(!is_writable_stream(strm) || !is_readable_stream(strm))
				return 0;
			if(is_full_stack(&sstrm->read_streams) && !expand_stack(&sstrm->read_streams))
				return 0;
			if(is_full_stack(&sstrm->write_streams) && !expand_stack(&sstrm->write_streams))
				return 0;
			return push_to_stack(&(sstrm->read_streams), strm) && push_to_stack(&(sstrm->write_streams), strm);
		}
		default :
			return 0;
	}
}

stream* get_top_of_stacked_stream(stacked_stream* sstrm, int operate_on)
{
	switch(operate_on)
	{
		case READ_STREAMS :
			return (stream*) get_top_of_stack(&(sstrm->read_streams));
		case WRITE_STREAMS :
			return (stream*) get_top_of_stack(&(sstrm->write_streams));
		default :
			return NULL;
	}
}

stream* get_nth_from_top_of_stacked_stream(stacked_stream* sstrm, unsigned int n, int operate_on)
{
	switch(operate_on)
	{
		case READ_STREAMS :
			return (stream*) get_nth_from_top_of_stack(&(sstrm->read_streams), n);
		case WRITE_STREAMS :
			return (stream*) get_nth_from_top_of_stack(&(sstrm->write_streams), n);
		default :
			return NULL;
	}
}

int pop_from_stacked_stream(stacked_stream* sstrm, int operate_on)
{
	switch(operate_on)
	{
		case READ_STREAMS :
			return pop_from_stack(&(sstrm->read_streams));
		case WRITE_STREAMS :
			return pop_from_stack(&(sstrm->write_streams));
		case BOTH_STREAMS :
		{
			if(get_element_count_stack(&sstrm->read_streams) == 0 || get_element_count_stack(&sstrm->write_streams) == 0)
				return 0;
			return pop_from_stack(&(sstrm->read_streams)) && pop_from_stack(&(sstrm->write_streams));
		}
		default :
			return 0;
	}
}

size_t read_from_stacked_stream(stacked_stream* sstrm, void* data, size_t data_size, int* error)
{
	return read_from_stream(get_top_of_stacked_stream(sstrm, READ_STREAMS), data, data_size, error);
}

int unread_from_stacked_stream(stacked_stream* sstrm, const void* data, size_t data_size)
{
	return unread_from_stream(get_top_of_stacked_stream(sstrm, READ_STREAMS), data, data_size);
}

size_t write_to_stacked_stream(stacked_stream* sstrm, const void* data, size_t data_size, int* error)
{
	return write_to_stream(get_top_of_stacked_stream(sstrm, WRITE_STREAMS), data, data_size, error);
}

void flush_all_from_stacked_stream(stacked_stream* sstrm, int* error)
{
	flush_all_from_stream(get_top_of_stacked_stream(sstrm, WRITE_STREAMS), error);
}