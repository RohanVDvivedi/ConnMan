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

	#define TRANSACTION_COUNT 13

	char* inpt_strs[TRANSACTION_COUNT] = {
			"Ready Server Brother",
			"hello, I am client",
			"ping",
			"exit",
			"world",
			"hey !! man",
			"This is main",
			"exit",
			"p0ng",
			"pong",
			"exit",
			"hola",
			"seniorita"
		};

	dstring inpt[TRANSACTION_COUNT] = {};
	promise* prms[TRANSACTION_COUNT] = {};

	for(int i = 0; i < TRANSACTION_COUNT; i++)
	{
		inpt[i] = (*dstring_DUMMY_CSTRING(inpt_strs[i]));
		prms[i] = queue_transaction(tcli, transaction, &inpt[i]);
		if(prms[i] == NULL)
			printf("could not queue promise %d\n", i);
	}

	printf("transactions queued successfully\n");

	for(int i = 0; i < TRANSACTION_COUNT; i++)
	{
		if(prms[i] != NULL)
		{
			dstring* output = get_result_for_transaction(prms[i]);
			printf("promise %d -> ", i);printf_dstring(&inpt[i]);printf(" : ");
			if(output != NULL)
			{
				printf_dstring(output);
				deinit_dstring(output);free(output);
			}
			printf("\n");
		}
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

	dstring* output = malloc(sizeof(dstring));
	init_dstring(output, buffer, buffreadlength);
	return output;
}

