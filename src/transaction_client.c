#include<transaction_client.h>

// returns things like "tcp://127.0.0.1:80"
void get_connection_group_identifier(dstring* identifer, connection_group* conn_grp_p)
{
	make_dstring_empty(identifer);
	switch(conn_grp_p->TRANSMISSION_PROTOCOL_TYPE)
	{
		case SOCK_STREAM :
		{
			append_to_dstring(identifer, "tcp");
			break;
		}
		case SOCK_DGRAM :
		{
			append_to_dstring(identifer, "udp");
			break;
		}
	}

	append_to_dstring(identifer, "://");

	for(int i = 3; i >= 0; i--)
	{
		char temp[10] = "";
		uint32_t ip_part_dec = (conn_grp_p->SERVER_ADDRESS >> (i * 8)) & (0x000000ff);
		sprintf(temp, "%u", ip_part_dec);
		append_to_dstring(identifer, temp);

		if(i != 0)
		{
			append_to_dstring(identifer, ".");
		}
	}

	append_to_dstring(identifer, ":");

	char temp[10] = "";
	sprintf(temp, "%u", conn_grp_p->PORT);
	append_to_dstring(identifer, temp);
}

unsigned long long int thread_id_hash(const int* thread_id)
{
	return ((unsigned long long int)(*thread_id));
}

int compare_thread_ids(const int* a, const int* b)
{
	return ((*a) - (*b));
}

connection_group* get_connection_group(sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT, executor* transaction_executor)
{
	connection_group* conn_grp_p = (connection_group*) malloc(sizeof(connection_group));

	conn_grp_p->transaction_executor = transaction_executor;

	conn_grp_p->thread_id_to_file_discriptor = get_hashmap(10, (unsigned long long int (*)(const void* key))(thread_id_hash), (int (*)(const void* key1, const void* key2))(compare_thread_ids), ELEMENTS_AS_RED_BLACK_BST);
	conn_grp_p->thread_id_to_file_discriptor_lock = get_rwlock();

	conn_grp_p->ADDRESS_FAMILY = ADDRESS_FAMILY;
	conn_grp_p->TRANSMISSION_PROTOCOL_TYPE = TRANSMISSION_PROTOCOL_TYPE;
	conn_grp_p->SERVER_ADDRESS = SERVER_ADDRESS;
	conn_grp_p->PORT = PORT;

	return conn_grp_p;
}

void add_transaction_to_connection_group(connection_group* conn_grp_p, void (*transaction)(int fd))
{

}

void delete_entry_operation(int* key, int* value, const void* ap)
{
	free(key);
	free(value);
}

void delete_connection_group(connection_group* conn_grp_p)
{
	// delete all the elements of the thread_id_to_file_discriptor hashmap
	for_each_entry_in_hash(conn_grp_p->thread_id_to_file_discriptor, (void (*)(const void*, const void*, const void*))(delete_entry_operation), NULL);

	delete_hashmap(conn_grp_p->thread_id_to_file_discriptor);
	delete_rwlock(conn_grp_p->thread_id_to_file_discriptor_lock);
	free(conn_grp_p);
}