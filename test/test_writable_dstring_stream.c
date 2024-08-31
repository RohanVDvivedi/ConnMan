#include<stdio.h>
#include<stdlib.h>

#include<stream_for_dstring.h>

#include<dstring.h>
#include<string.h>

char* input[10] = {
	"Hello, Rohan !!\n",
	"How are you?\n",
	"How are you doing?\n",
	"Get well soon, son.\n",
	"Thank you so much, Ms. Rupa.\n",
	"You are always welcomed here.\n",
	"I know that Ms. Rupa.\n",
	"I am so glad that you are here now.\n",
	"Yeah, its now or never.\n",
	"Praise the lord.\n"
};

// uncomment below line to flush only at the end
//#define FLUSH_ALL_AT_END

int main()
{
	stream strm;
	dstring str; init_empty_dstring(&str, 0);
	initialize_dstring_stream(&strm, &str);

	int error;

	for(int i = 0; i < sizeof(input)/sizeof(input[0]); i++)
	{
		write_to_stream(&strm, input[i], strlen(input[i]), &error);
		#ifndef FLUSH_ALL_AT_END
			flush_all_from_stream(&strm, &error);
		#endif
	}

	printf("before flushing : <" printf_dstring_format "> \n", printf_dstring_params(&str));

#ifdef FLUSH_ALL_AT_END
	flush_all_from_stream(&strm, &error);
#endif

	printf("after flushing : <" printf_dstring_format "> \n", printf_dstring_params(&str));

	close_stream(&strm, &error);

	deinitialize_stream(&strm);
	deinit_dstring(&str);

	return 0;
}