#include<transaction_client.h>

int transaction(int fd, void* additional_params);

int main()
{
	connection_group* cgp = get_connection_group_tcp_ipv4(0x7f000001, 6969);

	transaction_client* tcli = get_transaction_client(cgp, 3);

	queue_transaction(tcli, transaction, get_dstring("hello\r\n", 10));
	queue_transaction(tcli, transaction, get_dstring("ping\r\n", 10));
	queue_transaction(tcli, transaction, get_dstring("pong\r\n", 10));
	queue_transaction(tcli, transaction, get_dstring("world\r\n", 10));
	queue_transaction(tcli, transaction, get_dstring("hey !! man\r\n", 10));
	queue_transaction(tcli, transaction, get_dstring("This is main\r\n", 10));

	shutdown_transaction_client(tcli, 0);
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

