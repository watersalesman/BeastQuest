# BeastQuest

BeastQuest is an easy-to-use HTTP request library for C++ made by humans for
humans. This library is heavily inspired by [C++ Requests](https://github.com/whoshuu/cpr)
and [Python Requests](https://github.com/requests/requests). This was largely
created as a proof-of-concept project utilizing the [Boost Beast library](https://github.com/boostorg/beast)
that was introduced in Boost 1.66.

Features
---

Because Boost Beast only provides a protocol-level interface for HTTP, many
features have to be implemented from scratch. The following features are not
available at the moment:
* Timeout specification
* Callback interface
* Streaming requests
* Cookie support
* Proxy support
* SSL verification

But the following features are supported!
* GET, POST, DELETE, PUT, and PATCH requests
* SSL/TLS requests
* Asynchronous requests
* Custom headers
* Url encoded parameters
* Simple POST forms
* Multipart POST forms
* File POST upload
* Basic authentication

Build
---
BeastQuest currently only supports Linux and OS X. This will change as I figure
out a convenient way of handling dependencies on Windows.

This project uses CMake, so it is as simple as cloning the repo, adding a new
subdirectory to your CMake project, and using the provided variables:
```
# This will download and configure Boost and OpenSSL in the "deps" folder
set(BeastQuest_BUILDS_DEPS ON)

add_subdirectory(beastquest)

include_directories(${BeastQuest_INCLUDES})
target_link_libraries(your_target_here ${BeastQuest_LIBRARIES})
```

Tests use Google Test, which can be downloaded by running the following:
```
git submodule update --init
```
Tests can be built by specifying by specifying the build type as *Debug*. This
can be done by passing either `-DCMAKE_BUILD_TYPE=Debug` to `cmake` on the
command line or `set(CMAKE_BUILD_TYPE Debug)` in your `CMakeLists.txt`.

Third-Party Libraries
---

The following libraries are used in this project:

* [Boost Beast](https://github.com/boostorg/beast) - An HTTP and WebSocket
library built on Boost.Asio in C++11
* [OpenSSL](https://github.com/openssl/openssl) - A TLS/SSL and crypto library
* [Google Test](https://github.com/google/googletest) - A C++ testing framework
* [nlohmann's json](https://github.com/nlohmann/json) - A JSON library for Modern C++ (used for testing)
