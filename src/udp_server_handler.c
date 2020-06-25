#include<udp_server_handler.h>

#include<handler_wrapper_params.h>

static void* handler_wrapper(void* handler_wrapper_input_params_v_p)
{
	handler_wrapper_input_params* handler_data = ((handler_wrapper_input_params*)handler_wrapper_input_params_v_p);

	handler_data->handler(handler_data->fd);

	free(handler_data);
	return NULL;
}

int udp_server_handler(int listen_fd, void (*handler)(int conn_fd), unsigned int thread_count)
{
	executor* message_executor = get_executor(FIXED_THREAD_COUNT_EXECUTOR, thread_count, 0, NULL, NULL, NULL);

	// create as many number of jobs as the number of threads
	unsigned int jobs_left_to_be_created = thread_count;
	while(jobs_left_to_be_created)
	{
		submit_function(message_executor, handler_wrapper, get_new_handler_wrapper_input_params(listen_fd, handler));
		jobs_left_to_be_created--;
	}

	// we do not shutdown immediately for UDP, 
	// instead wait for all udp message handling jobs to complete 
	shutdown_executor(message_executor, 0);

	wait_for_all_threads_to_complete(message_executor);

	delete_executor(message_executor);

	return 0;
}