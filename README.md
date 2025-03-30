# ConnMan
This is a C library, for developing multithreaded networking applications.
It is essentially an abstraction layer over the POSIX sockets library, providing a handler-callback based network agnostic API.
It allows you to change the underlying protocol layers (tcp to udp or ipv4 to ipv6) without any change in application logic.
Additionally, it provides a stream interface to work with tcp sockets, along with ssl and compression support.

## Setup instructions
**Install dependencies :**
 * [Cutlery](https://github.com/RohanVDvivedi/Cutlery)
 * [PosixUtils](https://github.com/RohanVDvivedi/PosixUtils)
 * [BoomPar](https://github.com/RohanVDvivedi/BoomPar)
 * [openssl](https://github.com/openssl/openssl)  ($ sudo apt install libssl-dev)
 * [zlib](https://github.com/madler/zlib)      ($ sudo apt install zlib1g-dev)

**Download source code :**
 * `git clone https://github.com/RohanVDvivedi/ConnMan.git`

**Build from source :**
 * `cd ConnMan`
 * `make clean all`

**Install from the build :**
 * `sudo make install`
 * ***Once you have installed from source, you may discard the build by*** `make clean`

## Using The library
 * add `-lconnman -lboompar -lpthread -lcutlery -lssl -lcrypto -lz` linker flag, while compiling your application
 * do not forget to include appropriate public api headers as and when needed. this includes
   * `#include<connman/server.h>`
   * `#include<connman/client.h>`
   * `#include<connman/client_set.h>`
   * `#include<connman/comm_address.h>`
   * `#include<connman/ssl_ctx_helper.h>`
      Additionally the library also provides the following stream apis, to create appropriate streams as per use.
   * `#include<connman/file_descriptor_stream.h>`
   * `#include<connman/piped_stream.h>`
   * `#include<connman/ssl_stream.h>`
   * `#include<connman/writable_dstring_stream.h>`
   * `#include<connman/zlib_readable_decompression_stream.h>`
   * `#include<connman/zlib_writable_compression_stream.h>`
   * `#include<connman/read_until_dstring_stream.h>`
      Below is the list of headers that contain utility functions that can be used with the created streams.
   * `#include<connman/stream_util.h>`
      Below header container allows you to manage multiple streams each chained with an underlying stream managed as a stack.
   * `#include<connman/stacked_stream.h>`

## Instructions for uninstalling library

**Uninstall :**
 * `cd ConnMan`
 * `sudo make uninstall`

## Third party acknowledgements
 * *gzip and deflate compression streams, internally supported by [zlib](https://github.com/madler/zlib) checkout their website [here](https://zlib.net/).*
 * *SSL and TLS encryption for ssl-ed streams, internally supported by [openssl](https://github.com/openssl/openssl) checkout their website [here](https://www.openssl.org/).*