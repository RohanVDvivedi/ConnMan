#include<comm_address.h>

#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stddef.h>

comm_address new_comm_address(int PROTOCOL, sa_family_t ADDRESS_FAMILY, char* IP, uint16_t PORT)
{
	comm_address comm_addr;
	comm_addr.PROTOCOL = PROTOCOL;
	if(ADDRESS_FAMILY == AF_INET)
	{
		comm_addr.ADDRESS_ipv4.sin_family = ADDRESS_FAMILY;
		comm_addr.ADDRESS_ipv4.sin_port = htons(PORT);
		if(IP != NULL)
			inet_pton(ADDRESS_FAMILY, IP, &(comm_addr.ADDRESS_ipv4.sin_addr));
		else
			comm_addr.ADDRESS_ipv4.sin_addr.s_addr = INADDR_ANY;
	}
	else if(ADDRESS_FAMILY == AF_INET6)
	{
		comm_addr.ADDRESS_ipv6.sin6_family = ADDRESS_FAMILY;
		comm_addr.ADDRESS_ipv6.sin6_port = htons(PORT);
		if(IP != NULL)
			inet_pton(ADDRESS_FAMILY, IP, &(comm_addr.ADDRESS_ipv6.sin6_addr));
		else
			comm_addr.ADDRESS_ipv6.sin6_addr = in6addr_any;
	}
	return comm_addr;
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

unsigned int get_sockaddr_size(comm_address* comm_addr_p)
{
	switch(comm_addr_p->ADDRESS.sa_family)
	{
		case AF_INET :
			return sizeof(comm_addr_p->ADDRESS_ipv4);
		case AF_INET6 :
			return sizeof(comm_addr_p->ADDRESS_ipv6);
		default :
			return sizeof(comm_addr_p->ADDRESS);
	}
}