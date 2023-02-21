#ifndef READ_UNTIL_DSTRING_STREAM_H
#define READ_UNTIL_DSTRING_STREAM_H

#include<stream.h>
#include<dstring.h>

// this stream produces everything from the underlying_strm until a given dstring is encountered
// after which it remains in closed state
// the given read_until_dstr is consumed from the underlying stream, but it does not get produced

typedef struct read_until_dstring_stream_context read_until_dstring_stream_context;
struct read_until_dstring_stream_context
{
	stream* underlying_strm;

	dpipe matched_bytes;

	dstring* read_until_dstr;

	unsigned int* read_until_dst_spml;
};

// 0 implies an error
int initialize_stream_for_reading_until_dstring(stream* strm, stream* underlying_strm, const dstring* read_until_dstr);

// same as above function but with precalculated suffix prefic match lengths
int initialize_stream_for_reading_until_dstring2(stream* strm, stream* underlying_strm, const dstring* read_until_dstr, const unsigned int* read_until_dst_spml);

#endif