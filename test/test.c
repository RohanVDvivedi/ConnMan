#include<server.h>

#include<string.h>

void connection_handler(int conn_fd);

int main()
{
	serve_tcp_on_ipv4(6969, connection_handler);
	return 0;
}

void connection_handler(int conn_fd)
{
	// 1 kb buffer
	char buffer[1000];

	while(1)
	{
		int buffreadlength = recv(conn_fd, buffer, 999, 0);
		buffer[buffreadlength] = '\0';

		printf("recevived : %s length %lu\n", buffer, strlen(buffer));

		if(buffreadlength == -1)
		{
			continue;
		}
		else if(strcmp(buffer, "exit\r\n") == 0 || strcmp(buffer, "exit\n") == 0 || strcmp(buffer, "exit") == 0)
		{
			break;
		}
		else if(strcmp(buffer, "ping\r\n") == 0 || strcmp(buffer, "ping\n") == 0 || strcmp(buffer, "ping") == 0)
		{
			send(conn_fd, "pong", strlen("pong"), 0);
		}
		else if(strcmp(buffer, "pong\r\n") == 0 || strcmp(buffer, "pong\n") == 0 || strcmp(buffer, "pong") == 0)
		{
			send(conn_fd, "ping", strlen("ping"), 0);
		}
		else
		{
			send(conn_fd, buffer, buffreadlength, 0);
		}
	}
}