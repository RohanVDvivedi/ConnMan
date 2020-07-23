#ifndef HANDLER_WRAPPER_PARAMS_H
#define HANDLER_WRAPPER_PARAMS_H

typedef struct handler_wrapper_input_params handler_wrapper_input_params;
struct handler_wrapper_input_params
{
	int fd;
	void* additional_params;
	void (*handler)(int fd, void* additional_params);
};

static handler_wrapper_input_params* get_new_handler_wrapper_input_params(int fd, void* additional_params, void (*handler)(int fd, void* additional_params))
{
	handler_wrapper_input_params* handler_wrapper_input_params_p = (handler_wrapper_input_params*)malloc(sizeof(handler_wrapper_input_params));
	handler_wrapper_input_params_p->fd = fd;
	handler_wrapper_input_params_p->additional_params = additional_params;
	handler_wrapper_input_params_p->handler = handler;
	return handler_wrapper_input_params_p;
}

#endif