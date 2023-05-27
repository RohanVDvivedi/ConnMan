#include<read_until_dstring_stream.h>

#include<stdlib.h>

static unsigned int read_from_stream_context(void* stream_context, void* data, unsigned int data_size, int* error)
{
	read_until_dstring_stream_context* sc = stream_context;

	unsigned int data_size_res = 0;

	const char* read_until_dstr_data = get_byte_array_dstring(&(sc->read_until_dstr));
	const cy_uint read_until_dstr_size = get_char_count_dstring(&(sc->read_until_dstr));

	while(sc->matched_length < read_until_dstr_size)
	{
		while(get_bytes_readable_in_dpipe(&(sc->cached_bytes)) > sc->matched_length && data_size_res < data_size)
		{
			read_from_dpipe(&(sc->cached_bytes), data + data_size_res, 1, PARTIAL_ALLOWED);
			data_size_res++;
		}

		if(data_size_res == data_size)
			break;

		char c;
		int uerror = 0;
		unsigned int byte_read = read_from_stream(sc->underlying_strm, &c, 1, &uerror);
		if(uerror)
		{
			(*error) = UNDERLYING_STREAM_ERROR;
			break;
		}
		if(byte_read == 0)
		{
			(*error) = UNDERLYING_STREAM_FINISHED_BEFORE_READ_UNTIL_DSTRING;
			break;
		}

		write_to_dpipe(&(sc->cached_bytes), &c, 1, PARTIAL_ALLOWED);

		while(1)
		{
			if(c == read_until_dstr_data[sc->matched_length])
			{
				sc->matched_length++;
				break;
			}
			else if(sc->matched_length == 0)
				break;
			else
				sc->matched_length = sc->read_until_dstr_spml[sc->matched_length];
		}
	}

	return data_size_res;
}

static void close_stream_context(void* stream_context, int* error)
{
	// NOP
}

static void destroy_stream_context(void* stream_context)
{
	read_until_dstring_stream_context* sc = stream_context;
	free(sc->read_until_dstr_spml);
	deinit_dstring(&(sc->read_until_dstr));
	deinitialize_dpipe(&(sc->cached_bytes));
	free(sc);
}

static read_until_dstring_stream_context*  new_reading_until_dstring_stream_context(stream* underlying_strm, const dstring* read_until_dstr, cy_uint* read_until_dstr_spml)
{
	read_until_dstring_stream_context* sc = malloc(sizeof(read_until_dstring_stream_context));
	sc->underlying_strm = underlying_strm;
	sc->matched_length = 0;
	initialize_dpipe(&(sc->cached_bytes), get_char_count_dstring(read_until_dstr));
	sc->read_until_dstr = new_copy_dstring(read_until_dstr);
	sc->read_until_dstr_spml = read_until_dstr_spml;
	return sc;
}

int initialize_stream_for_reading_until_dstring(stream* strm, stream* underlying_strm, const dstring* read_until_dstr)
{
	// create suffix prefix match lengths for read_until_dtr
	cy_uint* read_until_dstr_spml = malloc(sizeof(cy_uint) * (get_char_count_dstring(read_until_dstr) + 1));
	get_prefix_suffix_match_lengths(read_until_dstr, read_until_dstr_spml);

	// create stream context
	read_until_dstring_stream_context* sc = new_reading_until_dstring_stream_context(underlying_strm, read_until_dstr, read_until_dstr_spml);

	// initialize stream
	initialize_stream(strm, sc, read_from_stream_context, NULL, close_stream_context, destroy_stream_context, NULL, DEFAULT_MAX_UNFLUSHED_BYTES_COUNT);

	return 1;
}

int initialize_stream_for_reading_until_dstring2(stream* strm, stream* underlying_strm, const dstring* read_until_dstr, const cy_uint* read_until_dstr_spml)
{
	// create a copy suffix prefix match lengths (read_until_dst_spml)
	cy_uint* read_until_dstr_spml_l = malloc(sizeof(cy_uint) * (get_char_count_dstring(read_until_dstr) + 1));
	for(cy_uint i = 0; i <= get_char_count_dstring(read_until_dstr); i++)
		read_until_dstr_spml_l[i] = read_until_dstr_spml[i];

	// create stream context
	read_until_dstring_stream_context* sc = new_reading_until_dstring_stream_context(underlying_strm, read_until_dstr, read_until_dstr_spml_l);

	// initialize stream
	initialize_stream(strm, sc, read_from_stream_context, NULL, close_stream_context, destroy_stream_context, NULL, 0);

	return 1;
}