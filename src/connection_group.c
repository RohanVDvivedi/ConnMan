#include<connection_group.h>

connection_group get_connection_group(int PROTOCOL, sa_family_t ADDRESS_FAMILY, uint16_t PORT, char* SERVER_ADDRESS)
{
	connection_group conn_grp;
	conn_grp.PROTOCOL = PROTOCOL;
	if(ADDRESS_FAMILY == AF_INET)
	{
		conn_grp.ADDRESS_ipv4.sin_family = ADDRESS_FAMILY;
		conn_grp.ADDRESS_ipv4.sin_port = htons(PORT);
		inet_pton(ADDRESS_FAMILY, SERVER_ADDRESS, &(conn_grp.ADDRESS_ipv4.sin_addr));
	}
	else if(ADDRESS_FAMILY == AF_INET6)
	{
		conn_grp.ADDRESS_ipv6.sin6_family = ADDRESS_FAMILY;
		conn_grp.ADDRESS_ipv6.sin6_port = htons(PORT);
		inet_pton(ADDRESS_FAMILY, SERVER_ADDRESS, &(conn_grp.ADDRESS_ipv6.sin6_addr));
	}
	return conn_grp;
}

// fast utils

connection_group get_connection_group_tcp_ipv4(char* SERVER_ADDRESS, uint16_t PORT)
{
	return get_connection_group(SOCK_STREAM,  AF_INET, PORT, SERVER_ADDRESS);
}

connection_group get_connection_group_tcp_ipv6(char* SERVER_ADDRESS, uint16_t PORT)
{
	return get_connection_group(SOCK_STREAM, AF_INET6, PORT, SERVER_ADDRESS);
}

connection_group get_connection_group_udp_ipv4(char* SERVER_ADDRESS, uint16_t PORT)
{
	return get_connection_group( SOCK_DGRAM,  AF_INET, PORT, SERVER_ADDRESS);
}

connection_group get_connection_group_udp_ipv6(char* SERVER_ADDRESS, uint16_t PORT)
{
	return get_connection_group( SOCK_DGRAM, AF_INET6, PORT, SERVER_ADDRESS);
}