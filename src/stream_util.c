#include<stream_util.h>

#include<stddef.h>
#include<dstring.h>
#include<cutlery_stds.h>

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

unsigned int read_uint64_from_stream(stream* rs, unsigned int radix, uint64_t* data, int* error)
{
	if((radix != BINARY) && (radix != OCTAL) && (radix != DECIMAL) && (radix != HEXADECIMAL))
	{
		(*error) = -1;
		return 0;
	}

	unsigned int max_bytes_to_read = 0;
	if(radix == BINARY)
		max_bytes_to_read = 64;
	else if(radix == OCTAL)
		max_bytes_to_read = 22;
	else if(radix == DECIMAL)
		max_bytes_to_read = 20;
	else if(radix == HEXADECIMAL)
		max_bytes_to_read = 16;

	(*data) = 0;

	unsigned int bytes_read = 0;
	while(bytes_read < max_bytes_to_read)
	{
		char byte;
		unsigned int byte_read = read_from_stream(rs, &byte, 1, error);

		if(byte_read == 0 || (*error))
			break;

		unsigned int digit = get_digit_from_char(byte, radix);

		if(digit != INVALID_INDEX)
		{
			bytes_read++;
			(*data) *= radix;
			(*data) += digit;
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

unsigned int skip_dstring_from_stream(stream* rs, const dstring* str_to_skip, int* error)
{
	const char* str_data = get_byte_array_dstring(str_to_skip);
	const unsigned int str_size = get_char_count_dstring(str_to_skip);

	unsigned int match_size = 0;

	for(; match_size < str_size; match_size++)
	{
		char byte;
		unsigned int byte_read = read_from_stream(rs, &byte, 1, error);

		if(byte_read == 0 || (*error))
			break;

		if(byte != str_data[match_size])
		{
			unread_from_stream(rs, &byte, 1);
			break;
		}
	}

	// upon mismatch unread all
	if(match_size < str_size)
	{
		unread_from_stream(rs, str_data, match_size);
		return 0;
	}

	return str_size; // same as match_size
}

dstring read_dstring_until_from_stream(stream* rs, const dstring* until_str, unsigned int* prefix_suffix_match_lengths_for_until_str, unsigned int max_bytes_to_read, int* error)
{
	const char* until_str_data = get_byte_array_dstring(until_str);
	const unsigned int until_str_size = get_char_count_dstring(until_str);

	dstring res = new_dstring(NULL, 0);

	// the limit set too low to even include until_str_size
	if(until_str_size > max_bytes_to_read)
		return res;

	// how many characters of until_str matches with suffix of res
	unsigned int match_length = 0;

	while(match_length < until_str_size && get_char_count_dstring(&res) < max_bytes_to_read)
	{
		char byte;
		unsigned int byte_read = read_from_stream(rs, &byte, 1, error);

		if(byte_read == 0 || (*error))
			break;

		// append the character we just read to the res
		concatenate_char(&res, byte);

		// evaluate the new match length
		while(1)
		{
			if(byte == until_str_data[match_length])
			{
				match_length++;
				break;
			}
			else
			{
				if(match_length == 0)
					break;
				else
					match_length = prefix_suffix_match_lengths_for_until_str[match_length];
			}
		}
	}

	// if we couldn't match with until_str
	if(match_length < until_str_size)
	{
		unread_from_stream(rs, get_byte_array_dstring(&res), get_char_count_dstring(&res));
		make_dstring_empty(&res);
		shrink_dstring(&res);
	}

	return res;
}