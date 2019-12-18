#include<transaction_client.h>

int transaction(int fd, void* additional_params);

int main()
{
	executor* transaction_executor = get_executor(FIXED_THREAD_COUNT_EXECUTOR, 3, -1);

	connection_group* conn_grp_p = get_connection_group(AF_INET, SOCK_STREAM, 0x7f000001, 80, transaction_executor);

	add_transaction_to_connection_group(conn_grp_p, transaction, get_dstring("hello\r\n", 10));
	add_transaction_to_connection_group(conn_grp_p, transaction, get_dstring("ping\r\n", 10));
	add_transaction_to_connection_group(conn_grp_p, transaction, get_dstring("pong\r\n", 10));
	add_transaction_to_connection_group(conn_grp_p, transaction, get_dstring("world\r\n", 10));
	add_transaction_to_connection_group(conn_grp_p, transaction, get_dstring("hey !! man\r\n", 10));
	add_transaction_to_connection_group(conn_grp_p, transaction, get_dstring("This is main\r\n", 10));

	wait_for_all_threads_to_complete(transaction_executor);
	delete_executor(transaction_executor);

	delete_connection_group(conn_grp_p);

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

