#include<comm_address.h>

#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stddef.h>
#include<string.h>
#include<netdb.h>

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

int lookup_by_name(const char* hostname, const char* service, int PROTOCOL, int IP_FAMILY, comm_address server_addr[], int server_addr_size)
{
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = IP_FAMILY;
	hints.ai_socktype = PROTOCOL;
	if(PROTOCOL != 0)
	{
		if(PROTOCOL == SOCK_STREAM)
			hints.ai_protocol = IPPROTO_TCP;
		else if(PROTOCOL == SOCK_DGRAM)
			hints.ai_protocol = IPPROTO_UDP;
	}

	struct addrinfo* results = NULL;

	int err = getaddrinfo(hostname, service, &hints, &results);
	if(err)
	{
		freeaddrinfo(results);
		return -1;
	}
	else if(results == NULL)
	{
		freeaddrinfo(results);
		return 0;
	}

	int res = 0;
	for(struct addrinfo* rest = results; res < server_addr_size && rest != NULL; rest = rest->ai_next)
	{
		if(rest->ai_family != AF_INET && rest->ai_family != AF_INET6)
			continue;
		if(rest->ai_socktype != SOCK_STREAM && rest->ai_socktype != SOCK_DGRAM)
			continue;
		server_addr[res].PROTOCOL = rest->ai_socktype;
		server_addr[res].ADDRESS.sa_family = rest->ai_family;
		memcpy(&(server_addr[res].ADDRESS), rest->ai_addr, get_sockaddr_size(&server_addr[res]));
		res++;
	}

	freeaddrinfo(results);

	return res;
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

void snprintf_comm_address(dstring* res_append, comm_address* comm_addr_p)
{
	switch(comm_addr_p->PROTOCOL)
	{
		case SOCK_DGRAM :
		{
			snprintf_dstring(res_append, "udp://");
			break;
		}
		case SOCK_STREAM :
		{
			snprintf_dstring(res_append, "udp://");
			break;
		}
	}

	char dest[80];
	snprintf_dstring(res_append, ":%s", inet_ntop(comm_addr_p->ADDRESS.sa_family, &(comm_addr_p->ADDRESS), dest, 80));

	switch(comm_addr_p->ADDRESS.sa_family)
	{
		case AF_INET:
		{
			snprintf_dstring(res_append, ":%"PRIu16, ntohs(comm_addr_p->ADDRESS_ipv4.sin_port));
			break;
		}
		case AF_INET6:
		{
			snprintf_dstring(res_append, ":%"PRIu16, ntohs(comm_addr_p->ADDRESS_ipv6.sin6_port));
			break;
		}
	}
}