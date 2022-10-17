gcc test_server.c -o server.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto
gcc test_client.c -o client.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto

gcc test_stream_server.c -o stream_server.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto
gcc test_stream_client.c -o stream_client.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto

# use below command to generate a self signed certificate
#openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -sha256 -days 365