#include<connman/tcp_server_stream_handler.h>

#include<cutlery/stream_for_file_descriptor.h>
#include<connman/stream_handler_wrapper_params.h>

#include<boompar/executor.h>

#include<posixutils/pthread_cond_utils.h>

#include<netinet/in.h>
#include<unistd.h>
#include<errno.h>

static void* stream_handler_wrapper(void* stream_handler_wrapper_input_params_v_p)
{
	stream_handler_wrapper_input_params* handler_data = ((stream_handler_wrapper_input_params*)stream_handler_wrapper_input_params_v_p);

	stream fd_strm;
	if(!initialize_stream_for_fd(&fd_strm, handler_data->fd))
	{
		close(handler_data->fd);
		return NULL;
	}

	stream ssl_strm;
	if(handler_data->ssl_ctx != NULL)
	{
		if(!initialize_stream_for_ssl_server_over_stream(&ssl_strm, handler_data->ssl_ctx, &fd_strm))
		{
			int close_error;
			close_stream(&fd_strm, &close_error); // no need to close fd now
			deinitialize_stream(&fd_strm);
			return NULL;
		}
	}

	// only if the streams were successfully initialized
	// then we may call the handler
	handler_data->stream_handler((handler_data->ssl_ctx == NULL) ? (&fd_strm) : (&ssl_strm), handler_data->additional_params);

	// phase 5
	// close the streams, and the underlying filedescriptor
	int close_error;
	if(handler_data->ssl_ctx != NULL)
	{
		close_stream(&ssl_strm, &close_error);
		deinitialize_stream(&ssl_strm);
	}
	close_stream(&fd_strm, &close_error);  // no need to close fd now
	deinitialize_stream(&fd_strm);

	free(handler_data);
	return NULL;
}

// to handle clean up of the connection on cancellation of the job
static void stream_handler_wrapper_on_cancellation_callback(void* stream_handler_wrapper_input_params_v_p)
{
	stream_handler_wrapper_input_params* handler_data = ((stream_handler_wrapper_input_params*)stream_handler_wrapper_input_params_v_p);
	close(handler_data->fd);
	free(handler_data);
}

int tcp_server_stream_handler(int listen_fd, void* additional_params, void (*stream_handler)(stream* strm, void* additional_params), unsigned int thread_count, SSL_CTX* ssl_ctx, uint64_t timeout_in_milliseconds)
{
	// phase 3
	// listenning on the socket file discriptor 
	int err = listen(listen_fd, DEFAULT_BACKLOG_QUEUE_SIZE);
	if(err == -1)
		return err;

	// start a cached thread pool executor
	executor* connection_executor = new_executor(CACHED_THREAD_POOL_EXECUTOR, thread_count, thread_count * 8, DEFAULT_NO_CONNECTION_THREAD_DESTROY_TIMEOUT_IN_MICRO_SECONDS, NULL, NULL, NULL, 0);
	if(connection_executor == NULL)
		return -100;

	// start accepting in loop
	struct sockaddr_in client_addr;		socklen_t client_len = sizeof(client_addr);
	while(1)
	{
		// phase 4
		// accept uses backlog queue connection and de-queues them 
		err = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
		if(err == -1)
		{
			// break the listenning loop, if the listen_fd file discriptor is closed
			if(errno == EBADF || errno == ECONNABORTED || errno == EINVAL || errno == ENOTSOCK || errno == EPERM)
				break;
		}
		int conn_fd = err;

		if(timeout_in_milliseconds != BLOCKING && timeout_in_milliseconds != NON_BLOCKING)
		{
			struct timeval tv = timespec_to_timeval(timespec_from_milliseconds(timeout_in_milliseconds));
			setsockopt(conn_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
			setsockopt(conn_fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
		}

		// serve the connection that has been accepted, submit it to executor, to assign a thread to it
		// here wait for 10 milliseconds to timeout job submission
		stream_handler_wrapper_input_params* handler_data = new_stream_handler_wrapper_input_params(conn_fd, ssl_ctx, additional_params, stream_handler);
		if(handler_data == NULL)
		{
			close(conn_fd);
			continue;
		}
		if(!submit_job_executor(connection_executor, stream_handler_wrapper, handler_data, NULL, stream_handler_wrapper_on_cancellation_callback, 10 * 1000))
		{
			free(handler_data);
			close(conn_fd);
			continue;
		}
	}

	shutdown_executor(connection_executor, 1);

	wait_for_all_executor_workers_to_complete(connection_executor);

	delete_executor(connection_executor);

	return 0;
}