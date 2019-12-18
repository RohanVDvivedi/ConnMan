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

	shutdown_executor(transaction_executor, 0);

	delete_executor(transaction_executor);

	delete_connection_group(conn_grp_p);

	return 0;
}

int transaction(int fd, void* additional_params)
{
	dstring* data_to_send = (dstring*) additional_params;



	// delete additional_params
	delete_dstring(data_to_send);

	return 0;
}

