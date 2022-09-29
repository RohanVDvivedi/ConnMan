#ifndef STRREAM_HANDLER_WRAPPER_PARAMS_H
#define STRREAM_HANDLER_WRAPPER_PARAMS_H

#include<stdlib.h> // malloc and free

#include<stream.h>

typedef struct stream_handler_wrapper_input_params stream_handler_wrapper_input_params;
struct stream_handler_wrapper_input_params
{
	read_stream* rs;
	write_stream* ws;
	void* additional_params;
	void (*stream_handler)(read_stream* rs, write_stream* ws, void* additional_params);
};

static stream_handler_wrapper_input_params* new_stream_handler_wrapper_input_params(read_stream* rs, write_stream* ws, void* additional_params, void (*stream_handler)(read_stream* rs, write_stream* ws, void* additional_params))
{
	stream_handler_wrapper_input_params* stream_handler_wrapper_input_params_p = (stream_handler_wrapper_input_params*)malloc(sizeof(stream_handler_wrapper_input_params));
	stream_handler_wrapper_input_params_p->rs = rs;
	stream_handler_wrapper_input_params_p->ws = ws;
	stream_handler_wrapper_input_params_p->additional_params = additional_params;
	stream_handler_wrapper_input_params_p->stream_handler = stream_handler;
	return stream_handler_wrapper_input_params_p;
}


#endif