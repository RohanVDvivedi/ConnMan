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