#include<transaction_client.h>

#include<stdio.h>
#include<dstring.h>

void* transaction(int fd, int* close_connection_requested, void* additional_params);

connection_group cgp;
int main()
{
	cgp = get_connection_group_tcp_ipv4("127.0.0.1", 6969);
	//cgp = get_connection_group_udp_ipv4("127.0.0.1", 6969);
	//cgp = get_connection_group_tcp_ipv6("::1", 6969);
	//cgp = get_connection_group_udp_ipv6("::1", 6968);

	transaction_client* tcli = get_transaction_client(cgp, 3);

	dstring input_0; init_dstring(&input_0, "Ready Server Brother");
	promise* promise_0 = queue_transaction(tcli, transaction, &input_0);

	dstring input_1; init_dstring(&input_1, "hello, I am client");
	promise* promise_1 = queue_transaction(tcli, transaction, &input_1);

	dstring input_2; init_dstring(&input_2, "ping");
	promise* promise_2 = queue_transaction(tcli, transaction, &input_2);

	dstring input_3; init_dstring(&input_3, "exit");
	promise* promise_3 = queue_transaction(tcli, transaction, &input_3);

	dstring input_4; init_dstring(&input_4, "world");
	promise* promise_4 = queue_transaction(tcli, transaction, &input_4);

	dstring input_5; init_dstring(&input_5, "hey !! man");
	promise* promise_5 = queue_transaction(tcli, transaction, &input_5);

	dstring input_6; init_dstring(&input_6, "This is main");
	promise* promise_6 = queue_transaction(tcli, transaction, &input_6);

	dstring input_7; init_dstring(&input_7, "exit");
	promise* promise_7 = queue_transaction(tcli, transaction, &input_7);

	dstring input_8; init_dstring(&input_8, "p0ng");
	promise* promise_8 = queue_transaction(tcli, transaction, &input_8);

	dstring input_9; init_dstring(&input_9, "pong");
	promise* promise_9 = queue_transaction(tcli, transaction, &input_9);

	dstring input_10; init_dstring(&input_10, "exit");
	promise* promise_10 = queue_transaction(tcli, transaction, &input_10);

	dstring input_11; init_dstring(&input_11, "hola");
	promise* promise_11 = queue_transaction(tcli, transaction, &input_11);

	dstring input_12; init_dstring(&input_12, "seniorita");
	promise* promise_12 = queue_transaction(tcli, transaction, &input_12);

	printf("transactions queued successfully\n");

	dstring* output = NULL;
	dstring* input = NULL;

	if(promise_0 != NULL)
	{
		output = get_result_for_transaction(promise_0);
		printf("promise0-> ");display_dstring(&input_0);printf(" : ");
		deinit_dstring(&input_0);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise0\n");
	}

	if(promise_1 != NULL)
	{
		output = get_result_for_transaction(promise_1);
		printf("promise1-> ");display_dstring(&input_1);printf(" : ");
		deinit_dstring(&input_1);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise1\n");
	}

	if(promise_2 != NULL)
	{
		output = get_result_for_transaction(promise_2);
		printf("promise2-> ");display_dstring(&input_2);printf(" : ");
		deinit_dstring(&input_2);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise2\n");
	}

	if(promise_3 != NULL)
	{
		output = get_result_for_transaction(promise_3);
		printf("promise3-> ");display_dstring(&input_3);printf(" : ");
		deinit_dstring(&input_3);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise3\n");
	}

	if(promise_4 != NULL)
	{
		output = get_result_for_transaction(promise_4);
		printf("promise4-> ");display_dstring(&input_4);printf(" : ");
		deinit_dstring(&input_4);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise4\n");
	}

	if(promise_5 != NULL)
	{
		output = get_result_for_transaction(promise_5);
		printf("promise5-> ");display_dstring(&input_5);printf(" : ");
		deinit_dstring(&input_5);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise5\n");
	}

	if(promise_6 != NULL)
	{
		output = get_result_for_transaction(promise_6);
		printf("promise6-> ");display_dstring(&input_6);printf(" : ");
		deinit_dstring(&input_6);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise6\n");
	}

	if(promise_7 != NULL)
	{
		output = get_result_for_transaction(promise_7);
		printf("promise7-> ");display_dstring(&input_7);printf(" : ");
		deinit_dstring(&input_7);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise7\n");
	}

	if(promise_8 != NULL)
	{
		output = get_result_for_transaction(promise_8);
		printf("promise8-> ");display_dstring(&input_10);printf(" : ");
		deinit_dstring(&input_8);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise8\n");
	}

	if(promise_9 != NULL)
	{
		output = get_result_for_transaction(promise_9);
		printf("promise9-> ");display_dstring(&input_11);printf(" : ");
		deinit_dstring(&input_9);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise9\n");
	}

	if(promise_10 != NULL)
	{
		output = get_result_for_transaction(promise_10);
		printf("promise10-> ");display_dstring(&input_10);printf(" : ");
		deinit_dstring(&input_10);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise10\n");
	}

	if(promise_11 != NULL)
	{
		output = get_result_for_transaction(promise_11);
		printf("promise11-> ");display_dstring(&input_11);printf(" : ");
		deinit_dstring(&input_11);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise11\n");
	}

	if(promise_12 != NULL)
	{
		output = get_result_for_transaction(promise_12);
		printf("promise12-> ");display_dstring(&input_12);printf(" : ");
		deinit_dstring(&input_12);
		if(output != NULL)
		{
			display_dstring(output);
			delete_dstring(output);
			output = NULL;
		}
		printf("\n");
	}
	else
	{
		printf("could not queue promise12\n");
	}

	printf("shutdown transaction client\n");

	shutdown_transaction_client(tcli);

	printf("delete transaction client\n");

	delete_transaction_client(tcli);

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

	return get_dstring_data(buffer, buffreadlength);
}

