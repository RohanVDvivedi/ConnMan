#include<comm_address.h>

#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stddef.h>

comm_address new_comm_address(int PROTOCOL, sa_family_t ADDRESS_FAMILY, char* IP, uint16_t PORT)
{
	comm_address conn_grp;
	conn_grp.PROTOCOL = PROTOCOL;
	if(ADDRESS_FAMILY == AF_INET)
	{
		conn_grp.ADDRESS_ipv4.sin_family = ADDRESS_FAMILY;
		conn_grp.ADDRESS_ipv4.sin_port = htons(PORT);
		if(IP != NULL)
			inet_pton(ADDRESS_FAMILY, IP, &(conn_grp.ADDRESS_ipv4.sin_addr));
		else
			conn_grp.ADDRESS_ipv4.sin_addr.s_addr = INADDR_ANY;
	}
	else if(ADDRESS_FAMILY == AF_INET6)
	{
		conn_grp.ADDRESS_ipv6.sin6_family = ADDRESS_FAMILY;
		conn_grp.ADDRESS_ipv6.sin6_port = htons(PORT);
		if(IP != NULL)
			inet_pton(ADDRESS_FAMILY, IP, &(conn_grp.ADDRESS_ipv6.sin6_addr));
		else
			conn_grp.ADDRESS_ipv6.sin6_addr = in6addr_any;
	}
	return conn_grp;
}

// fast utils

comm_address new_comm_address_tcp_ipv4(char* IP, uint16_t PORT)
{
	return new_comm_address(SOCK_STREAM,  AF_INET, IP, PORT);
}

comm_address new_comm_address_tcp_ipv6(char* IP, uint16_t PORT)
{
	return new_comm_address(SOCK_STREAM, AF_INET6, IP, PORT);
}

comm_address new_comm_address_udp_ipv4(char* IP, uint16_t PORT)
{
	return new_comm_address( SOCK_DGRAM,  AF_INET, IP, PORT);
}

comm_address new_comm_address_udp_ipv6(char* IP, uint16_t PORT)
{
	return new_comm_address( SOCK_DGRAM, AF_INET6, IP, PORT);
}