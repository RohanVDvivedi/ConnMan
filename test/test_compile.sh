gcc test_server.c -o server.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto
gcc test_client.c -o client.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto

gcc test_stream_server.c -o stream_server.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto
gcc test_stream_client.c -o stream_client.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto

gcc test_client_set.c -o client_set.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto

gcc test_piped_stream.c -o piped_stream.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto

gcc test_writable_dstring_stream.c -o wd_stream.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto

gcc test_stream_util.c -o stream_util.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto
# test stream util using
# printf "  123 ABCDABCDABCD rohanvipuldvivediqqABCDABCDABCD1234 " | ./stream_util.out

gcc test_read_until_dstring_stream.c -o ruds.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto
# printf "  123 ABCDABCDABCD rohanvipuldvivediqqABCDABCDABCD1234 --XLAMMXLA-- Rohan" | ./ruds.out

gcc test_zlib_streams.c -o zlib_stream.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto -lz

# use below command to generate a self signed certificate
#openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -sha256 -days 365