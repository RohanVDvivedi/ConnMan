#include<transaction_client.h>

void get_connection_identifier(dstring* identifer, sa_family_t ADDRESS_FAMILY, int TRANSMISSION_PROTOCOL_TYPE, uint32_t SERVER_ADDRESS, uint16_t PORT);

int main()
{
	dstring* url = get_dstring("", 10);
	get_connection_identifier(url, AF_INET, SOCK_STREAM, 0x7f000001, 80);
	printf("url : ");display_dstring(url);printf("\n");
	return 0;
}