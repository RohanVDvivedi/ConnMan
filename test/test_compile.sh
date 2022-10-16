gcc test_server.c -o server.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto
gcc test_client.c -o client.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto
#gcc test_transaction_client.c -o tclient.out -lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto