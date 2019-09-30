#include<server.h>

int serve(int DOMAIN, int TRANSMISSION_PROTOCOL_TYPE, sa_family_t ADDRESS_FAMILY, uint32_t SERVER_ADDRESS, unsigned long long int PORT, unsigned long long int BACKLOG_QUEUE_SIZE, void (*connection_handler)(int conn_fd))
{
	int err;

	// phase 1
	// file discriptor to socket
	err = socket(DOMAIN, TRANSMISSION_PROTOCOL_TYPE, 0);
	if(err == -1)
	{
		goto end;
	}
	int fd = err;

	struct sockaddr_in server_addr;
	server_addr.sin_family = ADDRESS_FAMILY;
	server_addr.sin_addr.s_addr = htonl(SERVER_ADDRESS);
	server_addr.sin_port = htons(PORT);

	// phase 2
	// bind server address struct with the file discriptor
	err = bind(fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err == -1)
	{
		goto end;
	}

	// phase 3
	// listenning on the socket file discriptor 
	err = listen(fd, BACKLOG_QUEUE_SIZE);
	if (err == -1)
	{
		goto end;
	}
	
	// start accepting in loop
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int conn_fd;
	while(1)
	{
		// phase 4
		// accept uses backlog queue connection and de-queues them 
		err = accept(fd, (struct sockaddr*)&client_addr, &client_len);
		if (err == -1)
		{
			continue;
		}
		conn_fd = err;

		// serve the connection that has been accepted
		connection_handler(conn_fd);

		// phase 5
		// closing client socket
		err = close(conn_fd);
		if (err == -1)
		{
			continue;
		}
	}

	// phase 6
	// closing server socket
	err = close(fd);
	if (err == -1)
	{
		goto end;
	}

	end: return err;
}