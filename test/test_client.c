#include<client.h>

#include<stdio.h>

void connection_handler(int conn_fd);

int main()
{
	// connect to server on local host
	connect_to_tcp_on_ipv4(0x7F000001, 6969, connection_handler);
	//connect_to_udp_on_ipv4(0x7F000001, 6969, datagram_handler);
	return 0;
}

void connection_handler(int conn_fd)
{
	char buffer[1000];

	while(1)
	{
		printf("self : ");
		scanf("%s", buffer);
		int buffsentlength = send(conn_fd, buffer, strlen(buffer), 0);
		if(buffsentlength == -1 || buffsentlength == 0)
		{
			break;
		}

		int buffreadlength = recv(conn_fd, buffer, 999, 0);
		if(buffreadlength == -1 || buffreadlength == 0)
		{
			break;
		}
		buffer[buffreadlength] = '\0';

		printf("server : %s\n", buffer);
	}
}