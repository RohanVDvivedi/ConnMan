#include<connman/comm_address.h>
#include<connman/client.h>

#include<string.h>
#include<stdio.h>
#include<unistd.h>

void connection_handler(int conn_fd);

int main()
{
	comm_address cgp;

	//cgp = new_comm_address_tcp_ipv4("127.0.0.1", 6969);
	//cgp = new_comm_address_udp_ipv4("127.0.0.1", 6969);
	cgp = new_comm_address_tcp_ipv6("::1", 6969);
	//cgp = new_comm_address_udp_ipv6("::1", 6969);

	int conn_fd = make_connection(&cgp, NULL, 3000); // 3 second timeout

	connection_handler(conn_fd);

	close(conn_fd);

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