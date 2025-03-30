#include<cutlery/stream_util.h>
#include<cutlery/stream_for_file_descriptor.h>

#include<stdio.h>
#include<stdlib.h>
#include<inttypes.h>

void print_unread_bytes(stream* rs)
{
	unsigned int bytes_to_read = get_bytes_readable_in_dpipe(&(rs->unread_data));
	char* unread_bytes = malloc(bytes_to_read);
	read_from_dpipe(&(rs->unread_data), unread_bytes, bytes_to_read, ALL_OR_NONE);
	printf("unread <%.*s>\n", bytes_to_read, unread_bytes);
	free(unread_bytes);
}

int is_end_char(int is_end_of_stream, char c, const void* cntxt)
{
	return is_end_of_stream || (c == ' ') || (c == '\n');
}

int main()
{
	stream rs, ws;
	initialize_stream_for_fd(&rs, 0);
	initialize_stream_for_fd(&ws, 1);

	int error = 0;

	write_to_stream_formatted(&ws, &error, "%s %d %c %u %f\n", "Rohan", 123, 'X', 123.123);
	if(error)
	{
		printf("error encountered white writing to output stream\n");
		return 0;
	}
	flush_all_from_stream(&ws, &error);
	if(error)
	{
		printf("error encountered white flushing to output stream\n");
		return 0;
	}

	printf("enter space and then a uint64_t number\n");

	unsigned int spaces_skipped = skip_whitespaces_from_stream(&rs, 16, &error);
	if(error)
	{
		printf("error skipping whitespaces\n");
		return 0;
	}
	printf("%u whitespaces were skipped\n", spaces_skipped);

	unsigned long long int num = 0;
	#define RADIX 8
	unsigned int bytes_read = read_unsigned_long_long_int_from_stream(&rs, RADIX, &num, &error);
	printf("%u bytes read trying to read a number\n", bytes_read);
	if(error)
	{
		printf("error reading uint64_t number\n");
		return 0;
	}
	printf("%llo %llu %llx number was read\n", num, num, num);

	printf("now enter the string 'ABCDABCDABCD' to continue\n");

	spaces_skipped = skip_whitespaces_from_stream(&rs, 16, &error);
	if(error)
	{
		printf("error skipping whitespaces\n");
		return 0;
	}
	printf("%u whitespaces were skipped\n", spaces_skipped);

	unsigned int skip_string_read = skip_dstring_from_stream(&rs, &get_dstring_pointing_to_literal_cstring("ABCDABCDABCD"), &error);
	if(error)
	{
		printf("error skipping 'ABCDABCDABCD'\n");
		return 0;
	}
	if(skip_string_read)
		printf("'ABCDABCDABCD' was skipped\n");
	else
	{
		printf("'ABCDABCDABCD' was not skipped\n");
		print_unread_bytes(&rs);
		return 0;
	}

	spaces_skipped = skip_whitespaces_from_stream(&rs, 16, &error);
	if(error)
	{
		printf("error skipping whitespaces\n");
		return 0;
	}
	printf("%u whitespaces were skipped\n", spaces_skipped);

	dstring until_str = get_dstring_pointing_to_literal_cstring("ABCDABCDABCD");
	cy_uint prefix_suffix_match_lengths_for_until_str[128];
	get_prefix_suffix_match_lengths(&until_str, prefix_suffix_match_lengths_for_until_str);
	dstring dstring_read = read_until_dstring_from_stream(&rs, &until_str, prefix_suffix_match_lengths_for_until_str, 50, &error);
	if(error)
	{
		printf("error reading until 'ABCDABCDABCD'\n");
		return 0;
	}
	if(is_empty_dstring(&dstring_read))
	{
		printf("max byte limit encountered OR until_str not found\n");
		print_unread_bytes(&rs);
	}
	else
		printf("we read : <"printf_dstring_format ">\n", printf_dstring_params(&dstring_read));
	deinit_dstring(&dstring_read);

	printf("reading until space or new line\n");
	int last_byte;
	dstring_read = read_until_any_end_chars_from_stream(&rs, is_end_char, NULL, &last_byte, 10, &error);
	if(error)
	{
		printf("error reading until space or newline");
		return 0;
	}
	if(is_empty_dstring(&dstring_read))
	{
		printf("max byte limit encountered OR space o new line not found\n");
		print_unread_bytes(&rs);
	}
	else
		printf("we read : <"printf_dstring_format ">\n", printf_dstring_params(&dstring_read));
	deinit_dstring(&dstring_read);

	deinitialize_stream(&rs);
	deinitialize_stream(&ws);

	return 0;
}