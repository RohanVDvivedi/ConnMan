#include<transaction_client.h>

// converts "tcp://127.0.0.1:80" to a randon hash number
unsigned long long int connection_hash(dstring* connection_identifier)
{
	return connection_identifier->bytes_occupied;
}

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