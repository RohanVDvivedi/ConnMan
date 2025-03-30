#include<connman/stacked_stream.h>

// the (read_streams and write_streams) stacks in the stacked_stream are initialized with capacity equal to DEFAULT_STACKED_STREAM_STACKS_CAPACITY
#define DEFAULT_STACKED_STREAM_STACKS_CAPACITY 8

int initialize_stacked_stream(stacked_stream* sstrm)
{
	if(!initialize_arraylist(&(sstrm->read_streams), DEFAULT_STACKED_STREAM_STACKS_CAPACITY))
		return 0;
	if(!initialize_arraylist(&(sstrm->write_streams), DEFAULT_STACKED_STREAM_STACKS_CAPACITY))
	{
		deinitialize_arraylist(&(sstrm->read_streams));
		return 0;
	}
	return 1;
}

void deinitialize_stacked_stream(stacked_stream* sstrm)
{
	deinitialize_arraylist(&(sstrm->read_streams));
	deinitialize_arraylist(&(sstrm->write_streams));
}

int is_empty_stacked_stream(stacked_stream* sstrm, int operate_on)
{
	switch(operate_on)
	{
		case READ_STREAMS :
			return is_empty_arraylist(&(sstrm->read_streams));
		case WRITE_STREAMS :
			return is_empty_arraylist(&(sstrm->write_streams));
		default :
			return 1;
	}
}

unsigned int get_stream_count_stacked_stream(stacked_stream* sstrm, int operate_on)
{
	switch(operate_on)
	{
		case READ_STREAMS :
			return get_element_count_arraylist(&(sstrm->read_streams));
		case WRITE_STREAMS :
			return get_element_count_arraylist(&(sstrm->write_streams));
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
			if(is_full_arraylist(&sstrm->read_streams) && !expand_arraylist(&sstrm->read_streams))
				return 0;
			return push_back_to_arraylist(&(sstrm->read_streams), strm);
		}
		case WRITE_STREAMS :
		{
			if(!is_writable_stream(strm))
				return 0;
			if(is_full_arraylist(&sstrm->write_streams) && !expand_arraylist(&sstrm->write_streams))
				return 0;
			return push_back_to_arraylist(&(sstrm->write_streams), strm);
		}
		case BOTH_STREAMS :
		{
			if(!is_writable_stream(strm) || !is_readable_stream(strm))
				return 0;
			if(is_full_arraylist(&sstrm->read_streams) && !expand_arraylist(&sstrm->read_streams))
				return 0;
			if(is_full_arraylist(&sstrm->write_streams) && !expand_arraylist(&sstrm->write_streams))
				return 0;
			return push_back_to_arraylist(&(sstrm->read_streams), strm) && push_back_to_arraylist(&(sstrm->write_streams), strm);
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
			return (stream*) get_back_of_arraylist(&(sstrm->read_streams));
		case WRITE_STREAMS :
			return (stream*) get_back_of_arraylist(&(sstrm->write_streams));
		default :
			return NULL;
	}
}

stream* get_from_top_of_stacked_stream(stacked_stream* sstrm, unsigned int index, int operate_on)
{
	switch(operate_on)
	{
		case READ_STREAMS :
			return (stream*) get_from_back_of_arraylist(&(sstrm->read_streams), index);
		case WRITE_STREAMS :
			return (stream*) get_from_back_of_arraylist(&(sstrm->write_streams), index);
		default :
			return NULL;
	}
}

int pop_from_stacked_stream(stacked_stream* sstrm, int operate_on)
{
	switch(operate_on)
	{
		case READ_STREAMS :
			return pop_back_from_arraylist(&(sstrm->read_streams));
		case WRITE_STREAMS :
			return pop_back_from_arraylist(&(sstrm->write_streams));
		case BOTH_STREAMS :
		{
			if(get_element_count_arraylist(&sstrm->read_streams) == 0 || get_element_count_arraylist(&sstrm->write_streams) == 0)
				return 0;
			return pop_back_from_arraylist(&(sstrm->read_streams)) && pop_back_from_arraylist(&(sstrm->write_streams));
		}
		default :
			return 0;
	}
}

cy_uint read_from_stacked_stream(stacked_stream* sstrm, void* data, cy_uint data_size, int* error)
{
	return read_from_stream(get_top_of_stacked_stream(sstrm, READ_STREAMS), data, data_size, error);
}

void unread_from_stacked_stream(stacked_stream* sstrm, const void* data, cy_uint data_size, int* error)
{
	return unread_from_stream(get_top_of_stacked_stream(sstrm, READ_STREAMS), data, data_size, error);
}

cy_uint write_to_stacked_stream(stacked_stream* sstrm, const void* data, cy_uint data_size, int* error)
{
	return write_to_stream(get_top_of_stacked_stream(sstrm, WRITE_STREAMS), data, data_size, error);
}

void flush_all_from_stacked_stream(stacked_stream* sstrm, int* error)
{
	flush_all_from_stream(get_top_of_stacked_stream(sstrm, WRITE_STREAMS), error);
}