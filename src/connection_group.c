#include<connection_group.h>

connection_group get_connection_group(int PROTOCOL, sa_family_t ADDRESS_FAMILY, uint16_t PORT)
{
	connection_group conn_grp;
	conn_grp.PROTOCOL = PROTOCOL;
	if(ADDRESS_FAMILY == AF_INET)
	{
		conn_grp.ADDRESS_ipv4.sin_family = ADDRESS_FAMILY;
		conn_grp.ADDRESS_ipv4.sin_port = htons(PORT);
	}
	else if(ADDRESS_FAMILY == AF_INET6)
	{
		conn_grp.ADDRESS_ipv6.sin6_family = ADDRESS_FAMILY;
		conn_grp.ADDRESS_ipv6.sin6_port = htons(PORT);
	}
	return conn_grp;
}

// fast utils

connection_group get_connection_group_tcp_ipv4(char* SERVER_ADDRESS, uint16_t PORT)
{
	connection_group conn_grp = get_connection_group(SOCK_STREAM, AF_INET, PORT);
	inet_aton(SERVER_ADDRESS, &(conn_grp.ADDRESS_ipv4.sin_addr));
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
	inet_aton(SERVER_ADDRESS, &(conn_grp.ADDRESS_ipv4.sin_addr));
	return conn_grp;
}

connection_group get_connection_group_udp_ipv6(char* SERVER_ADDRESS, uint16_t PORT)
{
	connection_group conn_grp = get_connection_group(SOCK_DGRAM, AF_INET6, PORT);
	//conn_grp.ADDRESS.sin_addr.s_addr = SERVER_ADDRESS;
	return conn_grp;
}