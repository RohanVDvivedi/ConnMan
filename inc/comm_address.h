#ifndef COMM_ADDRESS_H
#define COMM_ADDRESS_H

#include<netinet/in.h>	// structure definitions of sockaddr_*

#include<stdint.h>
#include<inttypes.h>
#include<dstring.h>

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

// returns number of comm_address-es set in server_addr
int lookup_by_name(const char* hostname, const char* service, int PROTOCOL, int IP_FAMILY, comm_address server_addr[], int server_addr_size);

// returns the size of the underlying socket_address stored as ADDRESS field
unsigned int get_sockaddr_size(comm_address* comm_addr_p);

// appends printable form of comm_address to res_append
void snprintf_comm_address(dstring* res_append, comm_address* comm_addr_p);

#endif