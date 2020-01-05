#include<transaction_client.h>

void* transaction(int fd, int* close_connection_requested, void* additional_params);

int main()
{
	connection_group* cgp = get_connection_group_tcp_ipv4(0x7f000001, 6969);

	transaction_client* tcli = get_transaction_client(cgp, 1);

	job* job_1 = queue_transaction(tcli, transaction, get_dstring("hello", 10));
	job* job_2 = queue_transaction(tcli, transaction, get_dstring("ping", 10));
	job* job_3 = queue_transaction(tcli, transaction, get_dstring("pong", 10));
	job* job_4 = queue_transaction(tcli, transaction, get_dstring("world", 10));
	job* job_5 = queue_transaction(tcli, transaction, get_dstring("hey !! man", 10));
	job* job_6 = queue_transaction(tcli, transaction, get_dstring("This is main", 10));

	printf("transactions queued successfully\n");

	dstring* output = NULL;
	dstring* input = NULL;

	if(job_1 != NULL)
	{
		output = get_result_for_transaction(job_1, (void**)&input);
		printf("job1-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
		if(input != NULL)
		{
			delete_dstring(input);
			input = NULL;
		}
		if(output != NULL)
		{
			delete_dstring(output);
			output = NULL;
		}
	}
	else
	{
		printf("could not queue job1\n");
	}

	if(job_2 != NULL)
	{
		output = get_result_for_transaction(job_2, (void**)&input);
		printf("job2-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
		if(input != NULL)
		{
			delete_dstring(input);
			input = NULL;
		}
		if(output != NULL)
		{
			delete_dstring(output);
			output = NULL;
		}
	}
	else
	{
		printf("could not queue job2\n");
	}

	if(job_3 != NULL)
	{
		output = get_result_for_transaction(job_3, (void**)&input);
		printf("job3-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
		if(input != NULL)
		{
			delete_dstring(input);
			input = NULL;
		}
		if(output != NULL)
		{
			delete_dstring(output);
			output = NULL;
		}
	}
	else
	{
		printf("could not queue job3\n");
	}

	if(job_4 != NULL)
	{
		output = get_result_for_transaction(job_4, (void**)&input);
		printf("job4-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
		if(input != NULL)
		{
			delete_dstring(input);
			input = NULL;
		}
		if(output != NULL)
		{
			delete_dstring(output);
			output = NULL;
		}
	}
	else
	{
		printf("could not queue job4\n");
	}

	if(job_5 != NULL)
	{
		output = get_result_for_transaction(job_5, (void**)&input);
		printf("job5-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
		if(input != NULL)
		{
			delete_dstring(input);
			input = NULL;
		}
		if(output != NULL)
		{
			delete_dstring(output);
			output = NULL;
		}
	}
	else
	{
		printf("could not queue job5\n");
	}

	if(job_6 != NULL)
	{
		output = get_result_for_transaction(job_6, (void**)&input);
		printf("job6-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
		if(input != NULL)
		{
			delete_dstring(input);
			input = NULL;
		}
		if(output != NULL)
		{
			delete_dstring(output);
			output = NULL;
		}
	}
	else
	{
		printf("could not queue job6\n");
	}

	printf("shutdown transaction client\n");

	shutdown_transaction_client(tcli);

	printf("delete transaction client\n");

	delete_transaction_client(tcli);

	printf("delete connection group\n");

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
		return NULL;
	}

	// read the data back, as a response
	char buffer[1000];
	int buffreadlength = recv(fd, buffer, 999, 0);
	if(buffreadlength == -1 || buffreadlength == 0)
	{
		(*close_connection_requested) = 1;
		return NULL;
	}
	buffer[buffreadlength] = '\0';

	return get_dstring(buffer, 10);
}

