#ifndef SSL_STREAM_BIO_H
#define SSL_STREAM_BIO_H

#include<cutlery/stream.h>

#include<openssl/bio.h>

BIO* get_new_bio_for_stream(stream* strm);

#endif