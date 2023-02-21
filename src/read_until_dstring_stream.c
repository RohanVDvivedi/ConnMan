#include<read_until_dstring_stream.h>

#include<stdlib.h>

static unsigned int read_from_stream_context(void* stream_context, void* data, unsigned int data_size, int* error)
{
	// TODO
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
	deinitialize_dpipe(&(sc->matched_bytes));
	free(sc);
}

static read_until_dstring_stream_context*  new_reading_until_dstring_stream_context(stream* underlying_strm, const dstring* read_until_dstr, unsigned int* read_until_dstr_spml)
{
	read_until_dstring_stream_context* sc = malloc(sizeof(read_until_dstring_stream_context));
	sc->underlying_strm = underlying_strm;
	initialize_dpipe(&(sc->matched_bytes), get_char_count_dstring(read_until_dstr) + 1);
	sc->read_until_dstr = new_copy_dstring(read_until_dstr);
	sc->read_until_dstr_spml = read_until_dstr_spml;
	return sc;
}

int initialize_stream_for_reading_until_dstring(stream* strm, stream* underlying_strm, const dstring* read_until_dstr)
{
	// create suffix prefix match lengths for read_until_dtr
	unsigned int* read_until_dstr_spml = malloc(sizeof(unsigned int) * (get_char_count_dstring(read_until_dstr) + 1));
	get_prefix_suffix_match_lengths(read_until_dstr, read_until_dstr_spml);

	// create stream context
	read_until_dstring_stream_context* sc = new_reading_until_dstring_stream_context(underlying_strm, read_until_dstr, read_until_dstr_spml);

	// initialize stream
	initialize_stream(strm, sc, read_from_stream_context, NULL, close_stream_context, destroy_stream_context, NULL);

	return 1;
}

int initialize_stream_for_reading_until_dstring2(stream* strm, stream* underlying_strm, const dstring* read_until_dstr, const unsigned int* read_until_dstr_spml)
{
	// create a copy suffix prefix match lengths (read_until_dst_spml)
	unsigned int* read_until_dstr_spml_l = malloc(sizeof(unsigned int) * (get_char_count_dstring(read_until_dstr) + 1));
	for(unsigned int i = 0; i <= get_char_count_dstring(read_until_dstr); i++)
		read_until_dstr_spml_l[i] = read_until_dstr_spml[i];

	// create stream context
	read_until_dstring_stream_context* sc = new_reading_until_dstring_stream_context(underlying_strm, read_until_dstr, read_until_dstr_spml_l);

	// initialize stream
	initialize_stream(strm, sc, read_from_stream_context, NULL, close_stream_context, destroy_stream_context, NULL);

	return 1;
}