#include<zlib_readable_decompression_stream.h>
#include<zlib_writable_compression_stream.h>
#include<piped_stream.h>

#include<executor.h>

#include<stdio.h>
#include<string.h>

stream pyp_strm;
stream zlib_comp;
stream zlib_decomp;
#define MAX_CAPACITY 12

char* input[10] = {
		"Hello, Rohan !!",
		"How are you?",
		"How are you doing?",
		"Get well soon, son.",
		"Thank you so much, Ms. Rupa.",
		"You are always welcomed here.",
		"I know that Ms. Rupa.",
		"I am so glad that you are here now.",
		"Yeah, its now or never.",
		"Praise the lord."
};

void* producer(void* param)
{
	int error;

	for(int i = 0; i < sizeof(input)/sizeof(input[0]); i++)
	{
		unsigned int data_size = strlen(input[i]);

		unsigned int bytes_written = write_to_stream(&zlib_comp, input[i], data_size, &error);
		if(error)
		{
			printf("producer : STREAM_ERROR(%d)\n", error);
			break;
		}

		printf("producer : <%s>(%u)\n", input[i], bytes_written);
	}

	// close stream once all the data has been written
	close_stream(&zlib_comp, &error);
	close_stream(&pyp_strm, &error);

	printf("producer : EXIT\n");

	return NULL;
}

#define CONSUMER_READ_BUFFER_SIZE 8

void* consumer(void* param)
{
	int error;

	char read_buffer[CONSUMER_READ_BUFFER_SIZE];

	while(1)
	{
		unsigned int bytes_read = read_from_stream(&zlib_decomp, read_buffer, CONSUMER_READ_BUFFER_SIZE, &error);

		if(bytes_read == 0 || error)
		{
			printf("consumer : STREAM_ERROR(%d) OR EOF(%d)\n", error, (bytes_read == 0));
			break;
		}

		printf("consumer : <%.*s>\n", bytes_read, read_buffer);
	}

	printf("consumer : EXIT\n");

	return NULL;
}

int main()
{
	initialize_piped_stream(&pyp_strm, MAX_CAPACITY);
	initialize_stream_for_zlib_compression(&zlib_comp, &pyp_strm, Z_BEST_COMPRESSION);
	initialize_stream_for_zlib_decompression(&zlib_decomp, &pyp_strm);

	// initialize job executor for executing transactions with the server
	executor* transaction_executor = new_executor(FIXED_THREAD_COUNT_EXECUTOR, 2, 10, 0, NULL, NULL, NULL);

	// submit consumer job
	submit_job(transaction_executor, consumer, NULL, NULL, 0);

	// submit producer job
	submit_job(transaction_executor, producer, NULL, NULL, 0);

	// shutdown, wait for shutdown and delete the job executor
	shutdown_executor(transaction_executor, 0);
	wait_for_all_threads_to_complete(transaction_executor);
	delete_executor(transaction_executor);

	// destroy stream
	deinitialize_stream(&pyp_strm);
	deinitialize_stream(&zlib_comp);
	deinitialize_stream(&zlib_decomp);
}