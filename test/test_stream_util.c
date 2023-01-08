#include<stream.h>
#include<stream_util.h>

#include<stdio.h>

int main()
{
	stream rs, ws;
	initialize_stream_for_fd(&rs, 0);
	initialize_stream_for_fd(&ws, 1);

	int error = 0;

	write_to_stream_formatted(&ws, "%s %d %c %u %f", &error, "Rohan", 123, 'X', 123.123);

	return 0;
}