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

# HTTP Requests

BeastQuest allows for a simple HTTP requests within C++ with a syntax similar
to the venerable [Python Requests](https://github.com/requests/requests)
library. This includes standards requests as well as asynchronous requests.

Options can be passed to either type of request in any order. One of these
options should be a `quest::Url` object, so we will stick with that for now.
For examples and a list of options, see the [Options](https://watersalesman.github.io/BeastQuest/options.html)
page.

## Response Object

HTTP requests return responses with headers and a body. These responses are
represented by the `quest::Response` struct, which is defined as shown below:

```c++
struct Response {
  Headers headers;      // Contains the response headers
  std::string content;  // Contains the response body
  int status_code;      // Contains the status code
  std::string reason;   // Contains the status reason
};
```

### Headers Object

The `quest::Headers` object is effectively a [std::map](http://en.cppreference.com/w/cpp/container/map)
of `std::string` keys and values. The only difference is that Headers is case
insensitive. Below is the definition:

```c++
using Headers = std::map<std::string, std::string, detail::CaseInsensitiveCompare>;
```

Case insensitivty for headers is required by [RFC 7230](https://tools.ietf.org/html/rfc7230#section-3.2).
Because of this, you can access headers like so:

```c++
quest::Headers headers = {{"key", "value"}};

std::cout << headers["key"] << std::endl;   // "value"
std::cout << headers["Key"] << std::endl;   // "value"
std::cout << headers["KEY"] << std::endl;   // "value"
```

## Synchronous Requests

The following request methods are supported:
* GET
* POST
* DELETE
* PUT
* PATCH

They can be called like so:

```c++
quest::Response res;

res = quest::Get(quest::Url("https://httpbin.org/get"));
res = quest::Post(quest::Url("https://httpbin.org/post"));
res = quest::Delete(quest::Url("https://httpbin.org/delete"));
res = quest::Put(quest::Url("https://httpbin.org/put"));
res = quest::Patch(quest::Url("https://httpbin.org/patch"));
```

## Asynchronous Requests

The asynchronous request API will wrap a standard request in a [std::async](http://en.cppreference.com/w/cpp/thread/async) 
function call. It returns a `quest::AsyncResponse` which is a [std::future](http://en.cppreference.com/w/cpp/thread/future)
of a `quest::Response` object as shown by the defintion below:

```c++
using AsyncResponse = std::future<Response>;
```

The async version of each request type is named `quest::Async*method-name*`,
e.g., `quest::AsyncGet` and `quest::AsyncPost`. These functions will take any
number of options, just like their synchronous counterparts.

An example usage of `quest::AsyncGet` is shown below:
```c++
auto response_future = quest::AsyncGet(quest::Url("https://httpbin.org/get"));

// Do other work...

auto response = response_future.get();
std::cout << response.content << std::endl;
```