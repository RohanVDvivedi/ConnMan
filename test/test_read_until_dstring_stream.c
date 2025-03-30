#include<cutlery/stream_to_read_until_dstring.h>

#include<cutlery/stream_for_file_descriptor.h>

#include<stdio.h>
#include<stdlib.h>

int main()
{
	stream rs, ws;
	initialize_stream_for_fd(&rs, 0);
	initialize_stream_for_fd(&ws, 1);

	int error = 0;

	dstring read_until_dstr = get_dstring_pointing_to_literal_cstring("XLAMMXLA");

	stream ruds;
	initialize_stream_for_reading_until_dstring(&ruds, &rs, &read_until_dstr);

	#define BUFFER_SIZE 8
	char buffer[BUFFER_SIZE];
	unsigned int buffer_size = BUFFER_SIZE;

	printf("reading with buffer size %u\n", BUFFER_SIZE);

	while(1)
	{
		buffer_size = BUFFER_SIZE;
		buffer_size = read_from_stream(&ruds, buffer, buffer_size, &error);

		if(error)
		{
			printf("%d error occured at <%.*s>\n", error, buffer_size, buffer);
			break;
		}
		if(buffer_size == 0)
		{
			printf("read until dstring encountered <" printf_dstring_format "> \n", printf_dstring_params(&read_until_dstr));
			break;
		}

		printf("->> read <%.*s>\n", buffer_size, buffer);
	}

	close_stream(&ruds, &error);
	deinitialize_stream(&ruds);

	deinitialize_stream(&rs);
	deinitialize_stream(&ws);

	return 0;
}