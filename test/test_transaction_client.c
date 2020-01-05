#include<transaction_client.h>

void* transaction(int fd, int* close_connection_requested, void* additional_params);

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

	printf("transactions queued successfully\n");

	dstring* output = NULL;
	dstring* input = NULL;

	output = get_result_for_transaction(job_1, (void**)&input);
	printf("job1-> \n");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	output = get_result_for_transaction(job_2, (void**)&input);
	printf("job2-> \n");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	output = get_result_for_transaction(job_3, (void**)&input);
	printf("job3-> \n");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	output = get_result_for_transaction(job_4, (void**)&input);
	printf("job4-> \n");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	output = get_result_for_transaction(job_5, (void**)&input);
	printf("job5-> \n");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	output = get_result_for_transaction(job_6, (void**)&input);
	printf("job6-> \n");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
	delete_dstring(input);
	input = NULL;
	delete_dstring(output);
	output = NULL;

	shutdown_transaction_client(tcli);
	delete_transaction_client(tcli);

	delete_connection_group(cgp);

	return 0;
}

void* transaction(int fd, int* close_connection_requested, void* additional_params)
{
	dstring* data_to_send = (dstring*) additional_params;

	// send data that needs to be sent
	int buffsentlength = send(fd, data_to_send->cstring, data_to_send->bytes_occupied, 0);
	if(buffsentlength == -1 || buffsentlength == 0)
	{
		(*close_connection_requested) = 1;
		return get_dstring("ERROR", 10);
	}

	// read the data back, as a response
	char buffer[1000];
	int buffreadlength = recv(fd, buffer, 999, 0);
	if(buffreadlength == -1 || buffreadlength == 0)
	{
		(*close_connection_requested) = 1;
		return get_dstring("ERROR", 10);
	}
	buffer[buffreadlength] = '\0';

	printf("job_completed\n");

	return get_dstring(buffer, 10);
}

