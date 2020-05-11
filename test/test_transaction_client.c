#include<transaction_client.h>

void* transaction(int fd, int* close_connection_requested, void* additional_params);

int main()
{
	connection_group* cgp = get_connection_group_tcp_ipv4(0x7f000001, 6969);

	transaction_client* tcli = get_transaction_client(cgp, 1);

	job* job_0 = queue_transaction(tcli, transaction, get_dstring("Ready Server Brother", 10));
	job* job_1 = queue_transaction(tcli, transaction, get_dstring("hello, I am client", 10));
	job* job_2 = queue_transaction(tcli, transaction, get_dstring("ping", 10));
	job* job_3 = queue_transaction(tcli, transaction, get_dstring("exit", 10));
	job* job_4 = queue_transaction(tcli, transaction, get_dstring("world", 10));
	job* job_5 = queue_transaction(tcli, transaction, get_dstring("hey !! man", 10));
	job* job_6 = queue_transaction(tcli, transaction, get_dstring("This is main", 10));
	job* job_7 = queue_transaction(tcli, transaction, get_dstring("exit", 10));
	job* job_8 = queue_transaction(tcli, transaction, get_dstring("p0ng", 10));
	job* job_9 = queue_transaction(tcli, transaction, get_dstring("pong", 10));
	job* job_10 = queue_transaction(tcli, transaction, get_dstring("exit", 10));
	job* job_11 = queue_transaction(tcli, transaction, get_dstring("hola", 10));
	job* job_12 = queue_transaction(tcli, transaction, get_dstring("seniorita", 10));

	printf("transactions queued successfully\n");

	dstring* output = NULL;
	dstring* input = NULL;

	if(job_0 != NULL)
	{
		output = get_result_for_transaction(job_0, (void**)&input);
		printf("job0-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
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
		printf("could not queue job0\n");
	}

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

	if(job_7 != NULL)
	{
		output = get_result_for_transaction(job_7, (void**)&input);
		printf("job7-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
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
		printf("could not queue job7\n");
	}

	if(job_8 != NULL)
	{
		output = get_result_for_transaction(job_8, (void**)&input);
		printf("job8-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
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
		printf("could not queue job8\n");
	}

	if(job_9 != NULL)
	{
		output = get_result_for_transaction(job_9, (void**)&input);
		printf("job9-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
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
		printf("could not queue job9\n");
	}

	if(job_10 != NULL)
	{
		output = get_result_for_transaction(job_10, (void**)&input);
		printf("job10-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
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
		printf("could not queue job10\n");
	}

	if(job_11 != NULL)
	{
		output = get_result_for_transaction(job_11, (void**)&input);
		printf("job11-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
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
		printf("could not queue job11\n");
	}

	if(job_12 != NULL)
	{
		output = get_result_for_transaction(job_12, (void**)&input);
		printf("job12-> ");display_dstring(input);printf(" : ");display_dstring(output);printf("\n");
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
		printf("could not queue job12\n");
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

