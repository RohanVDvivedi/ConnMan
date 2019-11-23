cd ..
make clean all
cd test
echo "compiling server"
gcc test_server.c -o server -I../inc -I../../BoomPar/inc -I../../cutlery/inc -L../bin -L../../BoomPar/bin -L../../cutlery/bin -lconnman -lboompar -lcutlery
echo "compiling client"
gcc test_client.c -o client -I../inc -I../../BoomPar/inc -I../../cutlery/inc -L../bin -L../../BoomPar/bin -L../../cutlery/bin -lconnman -lboompar -lcutlery
echo "compiling done"