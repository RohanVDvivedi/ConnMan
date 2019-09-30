cd ..
make clean all
cd test
gcc test.c -I../inc -I../../BoomPar/inc -I../../cutlery/inc -L../bin -L../../BoomPar/bin -L../../cutlery/bin -lconnman -lboompar -lcutlery