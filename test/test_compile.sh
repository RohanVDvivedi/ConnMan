echo "compiling server"
gcc test_server.c -o server.out -lpthread -lconnman -lboompar -lcutlery
echo "compiling client"
gcc test_client.c -o client.out -lpthread -lconnman -lboompar -lcutlery
echo "compiling transaction_client"
gcc test_transaction_client.c -o tclient.out -lpthread -lconnman -lboompar -lcutlery
echo "compiling done"
