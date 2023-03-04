#ifndef STREAM_H
#define STREAM_H

#include<dpipe.h>

typedef struct stream stream;
struct stream
{
	void* stream_context;

	// this pipe stores data that was unread_to_stream
	dpipe unread_data;

	// this pipe stores all data that has been written to the stream
	// but has not been flushed yet, using the write_to_stream_context function call
	dpipe unflushed_data;

	// this is the threshold of maximum unflushed_data that can exist in the unflushed_data pipe
	// beyond which a flush will be called, from the write call
	unsigned int max_unflushed_bytes_count;

	// returns bytes read from data, (atmost data_size number of bytes will be touched i.e. returned)
	// on error return 0 bytes read and set the value of (non-zero) error
	// reading 0 bytes, when data_size > 0 and strm->error == 0, then this implies an end_of_stream EOF
	// no read calls will be made after an EOF is received
	unsigned int (*read_from_stream_context)(void* stream_context, void* data, unsigned int data_size, int* error);

	// if this flag is set then, it implies that the last read call returned 0, i.e. EOF was received we call it end of stream
	// after this no read_from_stream_context calls will be made, only unread data will be returned
	// this flag is for internal use only
	int end_of_stream_received;

	// returns bytes written from data (that consists of data_size number of bytes to be written)
	// on error return 0 bytes read and set the value of (non-zero) error
	unsigned int (*write_to_stream_context)(void* stream_context, const void* data, unsigned int data_size, int* error);

	// closes the stream context
	void (*close_stream_context)(void* stream_context, int* error);

	// this function will be called to destroy the stream_context
	void (*destroy_stream_context)(void* stream_context);

	// An optional callback function, that gets called after a flush is performed on the stream
	void (*post_flush_callback_stream_context)(void* stream_context, int* error);

	// the error that was returned after the last call to read/write or close stream functions
	int last_error;
};

void initialize_stream(
						stream* strm, 
						void* stream_context,
						unsigned int (*read_from_stream_context)(void* stream_context, void* data, unsigned int data_size, int* error),
						unsigned int (*write_to_stream_context)(void* stream_context, const void* data, unsigned int data_size, int* error),
						void (*close_stream_context)(void* stream_context, int* error),
						void (*destroy_stream_context)(void* stream_context),
						void (*post_flush_callback_stream_context)(void* stream_context, int* error),
						unsigned int max_unflushed_bytes_count
					);

#define DEFAULT_MAX_UNFLUSHED_BYTES_COUNT 4096

int is_readable_stream(stream* strm);

int is_writable_stream(stream* strm);

// a return value of 0 from this function implies end of input/socket closed from the other end
// after that no more calls should be made and you must exit your read loop
unsigned int read_from_stream(stream* rs, void* data, unsigned int data_size, int* error);

// returns 1 on success else a 0
int unread_from_stream(stream* rs, const void* data, unsigned int data_size);

// return value of this function suggests, the number of bytes from data_size, that were either
// pushed to unflushed_data pipe, or actually wrriten to the stream_context (this happens if the incomming bytes and the unflushed_data bytes sum to more than max_unflushed_bytes_count)
unsigned int write_to_stream(stream* ws, const void* data, unsigned int data_size, int* error);

// flushes all the written data using the write_to_stream_context function call on the underlying stream context
void flush_all_from_stream(stream* ws, int* error);

void close_stream(stream* strm, int* error);

void deinitialize_stream(stream* strm);

// use UNDERLYING_STREAM_ERROR as error code when write_to_stream_context has failed,
// due to failure to write to an underlying stream (probably because of memory allocation failure)
#define UNDERLYING_STREAM_ERROR ((-1)<<15)

#endif