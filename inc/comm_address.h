#ifndef COMM_ADDRESS_H
#define COMM_ADDRESS_H

#include<netinet/in.h>	// structure definitions of sockaddr_*

#include<stdint.h>

// the comm_address is responsible to uniquely identify protocol used and the ip address and the port
typedef struct comm_address comm_address;
struct comm_address
{
	// specifies if the protocol is tcp or udp
	int PROTOCOL;

	// below union stores corresponding ipv4 or ipv6 address of the server
	union
	{
		struct sockaddr     ADDRESS;
		struct sockaddr_in  ADDRESS_ipv4;
		struct sockaddr_in6 ADDRESS_ipv6;
	};
};

// in the below functions, if IP == NULL, then IP = "0.0.0.0" or 

comm_address new_comm_address_tcp_ipv4(char* IP, uint16_t PORT);

comm_address new_comm_address_tcp_ipv6(char* IP, uint16_t PORT);

comm_address new_comm_address_udp_ipv4(char* IP, uint16_t PORT);

comm_address new_comm_address_udp_ipv6(char* IP, uint16_t PORT);

#endif