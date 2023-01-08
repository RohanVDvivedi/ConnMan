#include<stream_util.h>

#include<stddef.h>
#include<dstring.h>

unsigned int write_to_stream_formatted(stream* ws, const char* cstr_format, int* error, ...)
{
	dstring str = new_dstring(NULL, 0);

	va_list var_args;
	va_start(var_args, error);

	int dstringify_success = vsnprintf_dstring(&str, cstr_format, var_args);

	va_end(var_args);

	unsigned int bytes_written = 0;
	if(dstringify_success)
		bytes_written = write_to_stream(ws, get_byte_array_dstring(&str), get_char_count_dstring(&str), error);

	deinit_dstring(&str);

	return bytes_written;
}

unsigned int read_uint64_from_stream(stream* rs, uint64_t* data, int* error)
{
	(*data) = 0;

	unsigned int bytes_read = 0;
	while(bytes_read < 20)
	{
		char byte;
		unsigned int byte_read = read_from_stream(rs, &byte, 1, error);

		if(byte_read == 0 || (*error))
			break;

		if('0' <= byte && byte <= '9')
		{
			bytes_read++;
			(*data) *= 10;
			(*data) += (byte - '0');
		}
		else
		{
			unread_from_stream(rs, &byte, 1);
			break;
		}
	}

	return bytes_read;
}

unsigned int skip_whitespaces_from_stream(stream* rs, unsigned int max_whitespaces_to_skip, int* error)
{
	unsigned int bytes_read = 0;
	while(bytes_read < max_whitespaces_to_skip)
	{
		char byte;
		unsigned int byte_read = read_from_stream(rs, &byte, 1, error);

		if(byte_read == 0 || (*error))
			break;

		if(is_whitespace_char(byte))
			bytes_read++;
		else
		{
			unread_from_stream(rs, &byte, 1);
			break;
		}
	}

	return bytes_read;
}