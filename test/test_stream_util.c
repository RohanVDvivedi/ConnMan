#include<stream_util.h>
#include<file_descriptor_stream.h>

#include<stdio.h>
#include<inttypes.h>

int main()
{
	stream rs, ws;
	initialize_stream_for_fd(&rs, 0);
	initialize_stream_for_fd(&ws, 1);

	int error = 0;

	write_to_stream_formatted(&ws, "%s %d %c %u %f\n", &error, "Rohan", 123, 'X', 123.123);

	if(error)
	{
		printf("error encountered white writing to output stream\n");
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

	uint64_t num = 0;
	unsigned int bytes_read = read_uint64_from_stream(&rs, &num, &error);
	printf("%u bytes read trying to read a number\n", bytes_read);
	if(error)
	{
		printf("error reading uint64_t number\n");
		return 0;
	}
	printf("%"PRIu64 " number was read\n", num);

	printf("now enter the string 'ABCDABCDABCD' to continue\n");

	spaces_skipped = skip_whitespaces_from_stream(&rs, 16, &error);
	if(error)
	{
		printf("error skipping whitespaces\n");
		return 0;
	}
	printf("%u whitespaces were skipped\n", spaces_skipped);

	unsigned int skip_string_read = skip_dstring_from_stream(&rs, &get_literal_cstring("ABCDABCDABCD"), &error);
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
		return 0;
	}

	spaces_skipped = skip_whitespaces_from_stream(&rs, 16, &error);
	if(error)
	{
		printf("error skipping whitespaces\n");
		return 0;
	}
	printf("%u whitespaces were skipped\n", spaces_skipped);

	dstring until_str = get_literal_cstring("ABCDABCDABCD")
	unsigned int prefix_suffix_match_lengths_for_until_str[128];
	get_prefix_suffix_match_lengths(&until_str, prefix_suffix_match_lengths_for_until_str);
	dstring dstring_read = read_dstring_until_from_stream(&rs, &until_str, prefix_suffix_match_lengths_for_until_str, 50, &error);
	if(error)
	{
		printf("error reading until 'ABCDABCDABCD'\n");
		return 0;
	}
	if(is_empty_dstring(&dstring_read))
		printf("max byte limit encountered before until_str")
	else
		printf("we read : <"printf_dstring_format ">\n", printf_dstring_params(&dstring_read));
	deinit_dstring(&dstring_read);


	deinitialize_stream(&rs);
	deinitialize_stream(&ws);

	return 0;
}