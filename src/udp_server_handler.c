#include<connman/udp_server_handler.h>

#include<connman/handler_wrapper_params.h>

#include<boompar/executor.h>

static void* handler_wrapper(void* handler_wrapper_input_params_v_p)
{
	handler_wrapper_input_params* handler_data = ((handler_wrapper_input_params*)handler_wrapper_input_params_v_p);

	handler_data->handler(handler_data->fd, handler_data->additional_params);

	free(handler_data);
	return NULL;
}

int udp_server_handler(int listen_fd, void* additional_params, void (*handler)(int conn_fd, void* additional_params), unsigned int thread_count)
{
	executor* message_executor = new_executor(FIXED_THREAD_COUNT_EXECUTOR, thread_count, thread_count, 0, NULL, NULL, NULL, 0);
	if(message_executor == NULL)
		return -100;

	// create as many number of jobs as the number of threads
	unsigned int jobs_left_to_be_created = thread_count;
	while(jobs_left_to_be_created)
	{
		handler_wrapper_input_params* handler_data = new_handler_wrapper_input_params(listen_fd, additional_params, handler);
		if(handler_data != NULL && !submit_job_executor(message_executor, handler_wrapper, handler_data, NULL, NULL, 10 * 1000)) // if handler_data is created and the job failed to be submitted then we need to free handler_data
			free(handler_data);
		jobs_left_to_be_created--;
	}

	// we do not shutdown immediately for UDP, 
	// instead wait for all udp message handling jobs to complete 
	shutdown_executor(message_executor, 0);

	wait_for_all_executor_workers_to_complete(message_executor);

	delete_executor(message_executor);

	return 0;
}