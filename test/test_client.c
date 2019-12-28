#include<connection_group.h>
#include<client.h>

#include<stdio.h>

void connection_handler(int conn_fd);

int main()
{
	connection_group* cgp = NULL;

	cgp = get_connection_group_tcp_ipv4(0x7f000001, 6969);
	connect_to(cgp, connection_handler);

	//cgp = get_connection_group_udp_ipv4(0x7f000001, 6969);
	//connect_to(cgp, datagram_handler);

	delete_connection_group(cgp);
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