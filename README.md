# ConnMan
This is a C library, for developing multithreaded networking applications.
It is essentially an abstraction layer over the POSIX sockets library, providing a handler-callback based network agnostic API.
It allows you to change the underlying protocol layers (tcp to udp or ipv4 to ipv6) without any change in application logic.

## Setup instructions

**Download source code :**
 * `git clone https://github.com/RohanVDvivedi/ConnMan.git`

**Build from source :**
 * `cd ConnMan`
 * `make clean all`

**Install from the build :**
 * `sudo make install`
 * ***Once you have installed from source, you may discard the build by*** `make clean`

## Using The library
 * add `-lconnman -lboompar -lpthread -lcutlery` linker flag, while compiling your application
 * do not forget to include appropriate public api headers as and when needed. this includes
   * `#include<array.h>`
   * `#include<stack.h>`
   * `#include<queue.h>`
   * `#include<heap.h>`
   * `#include<linkedlist.h>`
   * `#include<bst.h>`
   * `#include<hashmap.h>`
   * `#include<dstring.h>`
   * `#include<bitmap.h>`

## Instructions for uninstalling library

**Uninstall :**
 * `cd ConnMan`
 * `sudo make uninstall`
