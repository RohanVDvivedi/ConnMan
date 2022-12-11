#include<client_set.h>

#include<executor.h>

// client set that will be used by all the jobs
client_set* cls = NULL;

void* transact_with_server(void* param)
{
	char* input = param;
	int input_len = strlen(input);

	// acquire a client stream from the client set cls

	// write input to the client stream

	// read response and write it to the stdout

	// release/return the client stream

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
	client_set* cls = new_client_set(const comm_address* server_addr_p, SSL_CTX* ssl_ctx, 2);

	// intialize parameter to all the jobs
	char* inputs[] = {
		"Hi\n",
		"ping\n",
		"pong\n",
		"Hello\n",
		"bye\n",
		"lol\n"
	};

	// enqueue all the jobs

	// shutdown, wait for shutdown and delete the job executor
	shutdown_executor(transaction_executor, 0);
	wait_for_all_threads_to_complete(transaction_executor);
	delete_executor(transaction_executor);

	// shutdown client_set
	shutdown_and_delete_client_set(cls);

	return 0;
}