* [Introduction](https://watersalesman.github.io/BeastQuest/index.html)
    * [Features](https://watersalesman.github.io/BeastQuest/index.html#features)
    * [Usage](https://watersalesman.github.io/BeastQuest/index.html#usage)
    * [Third-Party Libraries](https://watersalesman.github.io/BeastQuest/index.html#third-party-libraries)
* [Build](https://watersalesman.github.io/BeastQuest/build.html)
    * [CMake Build](https://watersalesman.github.io/BeastQuest/build.html#cmake-build)
* [HTTP Requests](https://watersalesman.github.io/BeastQuest/requests.html)
    * [Response Object](https://watersalesman.github.io/BeastQuest/requests.html#response-object)
    * [Synchronous Requests](https://watersalesman.github.io/BeastQuest/requests.html#synchronous-requests)
    * [Asynchronous Requests](https://watersalesman.github.io/BeastQuest/requests.html#asynchronous-requests)
* [Options](https://watersalesman.github.io/BeastQuest/options.html)
* [Session](https://watersalesman.github.io/BeastQuest/session.html)

# Introduction

BeastQuest is an easy-to-use HTTP request library for C++ made by humans for
humans. This library is heavily inspired by [C++ Requests](https://github.com/whoshuu/cpr)
and [Python Requests](https://github.com/requests/requests). This was largely
created as a proof-of-concept project utilizing the [Boost Beast library](https://github.com/boostorg/beast)
that was introduced in Boost 1.66.

# Features

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
* Custom request headers/body
* Url encoded parameters
* Simple POST forms
* Multipart POST forms
* File POST upload
* Basic authentication

## Usage

Using the synchronous API. doing a GET request is as simple as:

```c++
#include <beastquest/beastquest.hh>
#include <iostream>

int main() {
    quest::Url url("https://httpbin.org/get");
    auto response = quest::Get(url);

    if (response.status_code == 200)
        std::cout << response.content << std::endl;

    return 0;
}
```

The sync and async APIs are built on top of a [session-layer API](include/beastquest/session.hh)
that also aims to be simple and easy-to-use.

## Third-Party Libraries

The following libraries are used in this project:

* [Boost Beast](https://github.com/boostorg/beast) - An HTTP and WebSocket
library built on Boost.Asio in C++11
* [OpenSSL](https://github.com/openssl/openssl) - A TLS/SSL and crypto library
* [Google Test](https://github.com/google/googletest) - A C++ testing framework
* [nlohmann's json](https://github.com/nlohmann/json) - A JSON library for Modern C++ (used for testing)
