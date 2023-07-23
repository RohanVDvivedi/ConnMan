#include<client_set.h>
#include<ssl_ctx_helper.h>

#include<executor.h>

#include<string.h>
#include<stdio.h>
#include<stdlib.h>

// client set that will be used by all the jobs
client_set* cls = NULL;

#define BUFFER_SIZE 64

void* transact_with_server(void* param)
{
	char* input = param;
	int input_len = strlen(input);

	int error;

	// acquire a client stream from the client set cls
	stream* cli_strm = reserve_client(cls, 0);

	if(cli_strm == NULL)
	{
		printf("error reserving stream\n");
		return NULL;
	}

	// write input to the client stream
	write_to_stream(cli_strm, input, input_len, &error);
	if(error)
	{
		printf("error in writing to stream\n");
		return_client(cls, cli_strm, 1);
		return NULL;
	}

	flush_all_from_stream(cli_strm, &error);
	if(error)
	{
		printf("error in flushing to stream\n");
		return_client(cls, cli_strm, 1);
		return NULL;
	}

	// read response
	char output[BUFFER_SIZE + 1];
	int buffreadlength = read_from_stream(cli_strm, output, BUFFER_SIZE, &error);
	if(error)
	{
		printf("error in reading from stream\n");
		return_client(cls, cli_strm, 1);
		return NULL;
	}
	output[buffreadlength] = '\0';

	// write input and output to the stdout
	printf("%s -> %s\n", input, output);

	// release/return the client stream
	return_client(cls, cli_strm, 0);

	return NULL;
}

int main()
{
	// initialize job executor for executing transactions with the server
	executor* transaction_executor = new_executor(CACHED_THREAD_POOL_EXECUTOR, 16, 1024, 1000000, NULL, NULL, NULL);

	// setup server communication address
	comm_address cgp;
	//cgp = new_comm_address_tcp_ipv4("127.0.0.1", 6969);
	cgp = new_comm_address_tcp_ipv6("::1", 6969);

	// initialize ssl context
	SSL_CTX* ssl_ctx = NULL;
	// comment below 2 lines to not use ssl
	ssl_lib_init();
	ssl_ctx = get_ssl_ctx_for_client("./cert.pem", "./key.pem");

	// create a client set
	cls = new_client_set(&cgp, ssl_ctx, 2);

	// intialize parameter to all the jobs
	char* inputs[] = {
		"Hi",
		"ping",
		"pong",
		"Hello",
		"bye",
		"lol"
	};

	// enqueue all the jobs
	for(int i = 0; i < sizeof(inputs)/sizeof(inputs[0]); i++)
		submit_job_executor(transaction_executor, transact_with_server, inputs[i], NULL, NULL, 0);

	// shutdown, wait for shutdown and delete the job executor
	shutdown_executor(transaction_executor, 0);
	wait_for_all_executor_workers_to_complete(transaction_executor);
	delete_executor(transaction_executor);

	// shutdown client_set
	shutdown_and_delete_client_set(cls);

	return 0;
}