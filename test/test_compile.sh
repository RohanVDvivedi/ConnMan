cd ..
make clean all
cd test
echo "compiling server"
gcc test_server.c -o server.out -I../inc -I../../ReaderWriterLock/inc -I../../BoomPar/inc -I../../cutlery/inc -L../bin -L../../BoomPar/bin -L../../cutlery/bin -L../../ReaderWriterLock/bin -lconnman -lboompar -lcutlery -lrwlock
echo "compiling client"
gcc test_client.c -o client.out -I../inc -I../../ReaderWriterLock/inc -I../../BoomPar/inc -I../../cutlery/inc -L../bin -L../../BoomPar/bin -L../../cutlery/bin -L../../ReaderWriterLock/bin -lconnman -lboompar -lcutlery -lrwlock
echo "compiling transaction_client"
gcc test_transaction_client.c -o tclient.out -I../inc -I../../ReaderWriterLock/inc -I../../BoomPar/inc -I../../cutlery/inc -L../bin -L../../BoomPar/bin -L../../cutlery/bin -L../../ReaderWriterLock/bin -lconnman -lboompar -lcutlery -lrwlock
echo "compiling done"