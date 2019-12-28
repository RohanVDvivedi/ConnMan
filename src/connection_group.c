#include<connection_group.h>

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

connection_group* get_connection_group(int TRANSMISSION_PROTOCOL_TYPE, sa_family_t ADDRESS_FAMILY, uint32_t SERVER_ADDRESS, uint16_t PORT, executor* thread_pool)
{
	connection_group* conn_grp_p = (connection_group*) malloc(sizeof(connection_group));

	conn_grp_p->TRANSMISSION_PROTOCOL_TYPE = TRANSMISSION_PROTOCOL_TYPE;
	conn_grp_p->ADDRESS_FAMILY = ADDRESS_FAMILY;
	conn_grp_p->SERVER_ADDRESS = SERVER_ADDRESS;
	conn_grp_p->PORT = PORT;

	conn_grp_p->thread_pool = thread_pool;

	conn_grp_p->connection_mapping = get_connection_mapper();

	return conn_grp_p;
}

void delete_connection_group(connection_group* conn_grp_p)
{
	delete_connection_mapper(conn_grp_p->connection_mapping);
	free(conn_grp_p);
}