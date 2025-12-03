#include<connman/ssl_stream_bio.h>

#include<cutlery/cutlery_math.h>

static int stream_bio_create(BIO *b)
{
	BIO_set_init(b, 1);
	BIO_set_data(b, NULL);     // your custom struct pointer
	return 1;
}

static int stream_bio_destroy(BIO *b)
{
	return 1;
}

static int stream_bio_read(BIO *b, char *buf, int len)
{
	BIO_clear_retry_flags(b);

	stream* strm = BIO_get_data(b);

	// openssl reads large chunks and then goes to writes and then reads again, so how about we flush before every read ensuring that all prior writes are flushed?
	// subsequent small writes will only trigger a flush if enough bytes have been accumulated
	// subsequent small reads will not flush anything since there is nothing to flush post the first flush
	int error = 0;
	flush_all_from_stream(strm, &error);

	if(error)
		return -1;

	error = 0;
	cy_uint bytes_read = read_from_stream(strm, buf, min(len, CY_UINT_MAX), &error);

	if(error)
		return -1;

	return bytes_read;
}

static int stream_bio_write(BIO *b, const char *buf, int len)
{
	BIO_clear_retry_flags(b);

	stream* strm = BIO_get_data(b);

	int error = 0;
	cy_uint bytes_written = write_to_stream(strm, buf, min(len, CY_UINT_MAX), &error);

	if(error)
		return -1;

	return bytes_written;
}

static long stream_bio_ctrl(BIO *b, int cmd, long num, void *ptr)
{
	stream* strm = BIO_get_data(b);

	switch (cmd)
	{
		case BIO_CTRL_FLUSH:
		{
			int error = 0;
			flush_all_from_stream(strm, &error);

			if(error)
				return 0;

			return 1;
		}

		case BIO_CTRL_PENDING:
		{
			return min(get_bytes_readable_in_dpipe(&(strm->unread_data)), LONG_MAX);
		}

		case BIO_CTRL_WPENDING:
		{
			return min(get_bytes_readable_in_dpipe(&(strm->unflushed_data)), LONG_MAX);
		}

		case BIO_CTRL_EOF:
		{
			return strm->end_of_stream_received;
		}

		case BIO_CTRL_RESET:
		{
			return 1;   // indicate success, but do nothing
		}

		case BIO_CTRL_GET_CLOSE:
		{
			return BIO_get_shutdown(b);  // OK to pass through
		}

		case BIO_CTRL_SET_CLOSE:
		{
			BIO_set_shutdown(b, num);
			return 1;
		}

		case BIO_CTRL_PUSH:
		case BIO_CTRL_POP:
		case BIO_CTRL_DUP:
			return 1;   // required for compatibility

		default :
			return 0;
	}
	return 0;
}

static BIO_METHOD* stream_bio_method = NULL;

static void init_stream_bio_method()
{
	if(stream_bio_method != NULL)
		return;

    stream_bio_method = BIO_meth_new(BIO_TYPE_SOURCE_SINK, "stream_bio");
    BIO_meth_set_write(stream_bio_method, stream_bio_write);
    BIO_meth_set_read(stream_bio_method,  stream_bio_read);
    BIO_meth_set_ctrl(stream_bio_method,  stream_bio_ctrl);
    BIO_meth_set_create(stream_bio_method, stream_bio_create);
    BIO_meth_set_destroy(stream_bio_method, stream_bio_destroy);
}

BIO* get_new_bio_for_stream(stream* strm)
{
	init_stream_bio_method();

	BIO *b = BIO_new(stream_bio_method);
	BIO_set_data(b, strm);
	BIO_set_init(b, 1);

	return b;
}