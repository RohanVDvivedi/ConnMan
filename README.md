# ConnMan
This is a C library, for developing multithreaded networking applications.
It is essentially an abstraction layer over the POSIX sockets library, providing a handler-callback based network agnostic API.
It allows you to change the underlying protocol layers (tcp to udp or ipv4 to ipv6) without any change in application logic.

setup instructions
 * git clone https://github.com/RohanVDvivedi/ConnMan.git
 * cd ConnMan
 * sudo make clean install
 * add "-lconnman -lboompar -lpthread -lcutlery" linker flag, while compiling your application