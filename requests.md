# HTTP Requests

BeastQuest allows for a simple HTTP requests within C++ with a syntax similar
to the venerable [Python Requests](https://github.com/requests/requests)
library. This includes standards requests as well as asynchronous requests.

Options can be passed to either type of request in any order. One of these
options should be a `quest::Url` object, so we will stick with that for now.
For examples and a list of options, see the [Options]({{ site.baseurl }}/options.html)
page.

## Response Object

HTTP requests return responses with headers and a body. These responses are
represented by the `quest::Response` struct, which is defined like so:

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
of `std::string` keys and values. The only difference is that `quest::Headers`
is case-insensitive. Below is the definition:

```c++
using Headers = std::map<std::string, std::string, detail::CaseInsensitiveCompare>;
```

Case-insensitivty for headers is required by [RFC 7230](https://tools.ietf.org/html/rfc7230#section-3.2).
Because of this, you can access headers like so:

{% raw %}

```c++
quest::Headers headers{{"key", "value"}};

std::cout << headers["key"] << std::endl;   // "value"
std::cout << headers["Key"] << std::endl;   // "value"
std::cout << headers["KEY"] << std::endl;   // "value"
```

{% endraw %}

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

The asynchronous request API wraps a standard request in a [std::async](http://en.cppreference.com/w/cpp/thread/async)
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
auto res_f = quest::AsyncGet(quest::Url("https://httpbin.org/get"));

// Do other work...

auto response = res_f.get();    // Blocks until response is ready
std::cout << response.content << std::endl;
```

## Asynchronous Callbacks

BeastQuest supports passing a callback function to asynchronous requests. The
functions for the callback interface are named `quest::Callback*method-name*`,
e.g., `quest::CallbackGet` and `quest::CallbackPost`.

You can pass any function object as long as the first parameter of that function
object takes a `quest::Response` (or references such as `const quest::Response&`)
. Thanks to template programming and `auto` deduction, it doesn't even matter
what type it returns!

Once the request completes, and it will pass the `quest::Response` to your
provided callback and execute it. An example usage is shown below:

```c++
// Returns std::future of whatever your callback returns.
// In this case, it returns a std::future<std::string>
auto res_body_f = quest::CallbackGet([](quest::Response res){
    return res.content;
  }, quest::Url("https://httpbin.org/get"));

// Do other work...

// Blocks until ready, then prints returned std::string
std::cout << res_body_f.get() << std::endl;
```