#include<transaction_client.h>

int transaction(int fd, void* additional_params);

int main()
{
	connection_group* cgp = get_connection_group_tcp_ipv4(0x7f000001, 6969);

	transaction_client* tcli = get_transaction_client(cgp, 3);

	job* job_1 = queue_transaction(tcli, transaction, get_dstring("hello\r\n", 10));
	job* job_2 = queue_transaction(tcli, transaction, get_dstring("ping\r\n", 10));
	job* job_3 = queue_transaction(tcli, transaction, get_dstring("pong\r\n", 10));
	job* job_4 = queue_transaction(tcli, transaction, get_dstring("world\r\n", 10));
	job* job_5 = queue_transaction(tcli, transaction, get_dstring("hey !! man\r\n", 10));
	job* job_6 = queue_transaction(tcli, transaction, get_dstring("This is main\r\n", 10));

	dstring* output = NULL;
	dstring* input = NULL;

	output = get_result_for_transaction(job_1, (void**)&input);
	printf("-> ");display_dstring(input);printf(" : ");display_dstring(input);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	output = get_result_for_transaction(job_2, (void**)&input);
	printf("-> ");display_dstring(input);printf(" : ");display_dstring(input);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	output = get_result_for_transaction(job_3, (void**)&input);
	printf("-> ");display_dstring(input);printf(" : ");display_dstring(input);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	output = get_result_for_transaction(job_4, (void**)&input);
	printf("-> ");display_dstring(input);printf(" : ");display_dstring(input);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	output = get_result_for_transaction(job_5, (void**)&input);
	printf("-> ");display_dstring(input);printf(" : ");display_dstring(input);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	output = get_result_for_transaction(job_6, (void**)&input);
	printf("-> ");display_dstring(input);printf(" : ");display_dstring(input);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	shutdown_transaction_client(tcli);
	delete_transaction_client(tcli);

	delete_connection_group(cgp);

	return 0;
}

int transaction(int fd, void* additional_params)
{
	dstring* data_to_send = (dstring*) additional_params;

	// send data that needs to be sent
	int buffsentlength = send(fd, data_to_send->cstring, data_to_send->bytes_occupied, 0);
	if(buffsentlength == -1 || buffsentlength == 0)
	{
		return -1;
	}

	// read the data back, as a response
	char buffer[1000];
	int buffreadlength = recv(fd, buffer, 999, 0);
	if(buffreadlength == -1 || buffreadlength == 0)
	{
		return -1;
	}
	buffer[buffreadlength] = '\0';

	printf("server : %s\n", buffer);

	// delete additional_params
	delete_dstring(data_to_send);

	return 0;
}

