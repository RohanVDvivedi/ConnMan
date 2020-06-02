#include<connection_group.h>

// returns things like "tcp://127.0.0.1:80"
void get_connection_group_identifier(dstring* identifer, connection_group* conn_grp_p)
{
	make_dstring_empty(identifer);
	switch(conn_grp_p->PROTOCOL)
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
		uint32_t ip_part_dec = (( ntohl((conn_grp_p->ADDRESS).sin_addr.s_addr) ) >> (i * 8)) & (0x000000ff);
		sprintf(temp, "%u", ip_part_dec);
		append_to_dstring(identifer, temp);

		if(i != 0)
		{
			append_to_dstring(identifer, ".");
		}
	}

	append_to_dstring(identifer, ":");

	char temp[10] = "";
	sprintf(temp, "%u", ntohs(conn_grp_p->ADDRESS.sin_port));
	append_to_dstring(identifer, temp);
}

connection_group get_connection_group(int PROTOCOL, sa_family_t ADDRESS_FAMILY, uint16_t PORT)
{
	connection_group conn_grp;
	conn_grp.PROTOCOL = PROTOCOL;
	conn_grp.ADDRESS.sin_family = ADDRESS_FAMILY;
	conn_grp.ADDRESS.sin_port = htons(PORT);
	return conn_grp;
}

void delete_connection_group(connection_group* conn_grp_p)
{
	free(conn_grp_p);
}

// fast utils

connection_group get_connection_group_tcp_ipv4(char* SERVER_ADDRESS, uint16_t PORT)
{
	connection_group conn_grp = get_connection_group(SOCK_STREAM, AF_INET, PORT);
	inet_aton(SERVER_ADDRESS, &(conn_grp.ADDRESS.sin_addr));
	return conn_grp;
}

connection_group get_connection_group_tcp_ipv6(char* SERVER_ADDRESS, uint16_t PORT)
{
	connection_group conn_grp = get_connection_group(SOCK_STREAM, AF_INET6, PORT);
	//conn_grp.ADDRESS.sin_addr.s_addr = SERVER_ADDRESS;
	return conn_grp;
}

connection_group get_connection_group_udp_ipv4(char* SERVER_ADDRESS, uint16_t PORT)
{
	connection_group conn_grp = get_connection_group(SOCK_DGRAM, AF_INET, PORT);
	inet_aton(SERVER_ADDRESS, &(conn_grp.ADDRESS.sin_addr));
	return conn_grp;
}

connection_group get_connection_group_udp_ipv6(char* SERVER_ADDRESS, uint16_t PORT)
{
	connection_group conn_grp = get_connection_group(SOCK_DGRAM, AF_INET6, PORT);
	//conn_grp.ADDRESS.sin_addr.s_addr = SERVER_ADDRESS;
	return conn_grp;
}