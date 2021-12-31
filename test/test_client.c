#include<connection_group.h>
#include<client.h>

#include<string.h>
#include<stdio.h>

void connection_handler(int conn_fd);

int main()
{
	connection_group cgp;

	cgp = new_connection_group_tcp_ipv4("127.0.0.1", 6969);
	connect_to(&cgp, connection_handler);

	//cgp = new_connection_group_udp_ipv4("127.0.0.1", 6969);
	//connect_to(&cgp, connection_handler);

	//cgp = new_connection_group_tcp_ipv6("::1", 6969);
	//connect_to(&cgp, connection_handler);

	//cgp = new_connection_group_udp_ipv6("::1", 6968);
	//connect_to(&cgp, connection_handler);

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