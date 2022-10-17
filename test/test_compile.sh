gcc test_server.c -o server.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto
gcc test_client.c -o client.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto

gcc test_stream_server.c -o stream_server.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto