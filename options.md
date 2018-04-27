# Options

BeastQuest allows you to pass any options to a single request that you might
want to pass to a session. The options API is largely based on [C++ Requests's](https://github.com/whoshuu/cpr)
excellent API. The only requirement is that one of these options is a
`quest::Url` object. Other than that, you can pass any number of options in any
order! An example is shown below:

{% raw %}

```c++
quest::Response res;

// Valid request
res = quest::Get(quest::Url("https://httpbin.org/get"),
                 quest::Parameters{{"key1", "value1"}, {"key2", "value2"}},
                 quest::BasicAuth("user", "password"));

// Also a valid request
res = quest::Get(quest::BasicAuth("user", "password")),
                 quest::Url("https://httpbin.org/get"),
                 quest::Parameters{{"key1", "value1"}, {"key2", "value2"}};
```

{% endraw %}

## Option Types

### Url

The `quest::Url` struct allows you to pass a URL to a request or
`quest::Session` instance. The URL can include a port number as well as URL
parameters. Examples are shown below:

```c++
quest::Response res;

res = quest::Get(quest::Url("https://httpbin.org/get"));
res = quest::Get(quest::Url("https://httpbin.org/get?key=value"));
res = quest::Get(quest::Url("http://example.com:8080"));
```

Since `quest::Url` parses the URL string to get the needed components -- such as
the schema and hostname -- a valid URL must be inputted. Otherwise, the object
will throw a `std::runtime_error`. If there is a possibility of an invalid URL
being inputted into a `quest::Url` object, you may wish to wrap it in a
try-catch block like so:

```c++
// Assume that GetUrlString function exists and returns a std::string

quest::Url url;

try {
    url = quest::Url(GetUrlString());
} catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;     // Prints error about invalid URL
    // Handle error here...
}
```

### MaxRedirects

`quest::MaxRedirects` can be constructed from an int. This will, as the name suggests, limit the number of redirects that a request follows. Setting it to -1 will allow it to
follow infinite redirects (this is the default).

```c++
// Will only redirect twice
auto res = quest::Get("https://httpbin.org/redirect/4", quest::MaxRedirects(2));
```

### UserAgent

You can specify your request's [user agent](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/User-Agent)
with `quest::UserAgent`. It can be constructed from a
`std::string`.

```c++
auto res = quest::Get("https://httpbin.org/get", quest::UserAgent("My User Agent"));
```

### Headers

As mentioned in [Http Requests]({{ site.baseurl }}/requests.html#response-object)
, the `quest::Headers` object is a case-insensitive `std::map<std::string, std::string>`.
Therefore, it has all the [member functions](http://en.cppreference.com/w/cpp/container/map)
of a `std::map`. It can also be constructed using the convenient syntax of
[initializer lists](http://en.cppreference.com/w/cpp/utility/initializer_list)
as shown below:

{% raw %}

```c++
// All valid ways of constructing quest::Headers with initializer lists
quest::Headers h1{{"key1", "value1"}, {"key2", "value2"}};

quest::Headers h2 = {{"key1", "value1"}, {"key2", "value2"}};

auto response = quest::Get(quest::Url("https://httpbin.org/headers"),
                           quest::Headers{{"key", "value"}});
```

{% endraw %}

### Body

This is a simple struct with only a single `std::string`. It can therefore be
constructed with a simple `std::string` as shown below:

```c++
auto res = quest::Post(quest::Url("https://httpbin.org/post"),
                       quest::Body("My custom body"));
```

### BasicAuth

This class allows the passing of basic authentication credentials through a
request. It can be constructed from two `std::strings` representing the username
and password as shown below:

```c++
auto res = quest::Get(quest::Url("https://httpbin.org/basic-auth/user/pass"),
                      quest::BasicAuth("user", "pass"));
```

### Parameters

The `quest::Parameters` struct can be used to express URL parameters, such as
"key" and "value" in `http://example.com/?key=value`. This object can be
constructed from `quest::Parameter` objects, an alias for `std::pair<std::string, std::string>`.
Construction can also take advantage of initializer lists as shown in the
examples below:

{% raw %}

```c++
quest::Parameters params{{"key1", "value1"}, {"key2", "key2"}};

// Add additional parameter
quest::Parameter new_param{"key3", "value3"};
params.AddParameter(new_param);

auto res = quest::Get(quest::Url("https://httpbin.org/get"), params);
```

{% endraw %}

### SimpleForm

The `quest::SimpleForm` object behaves almost identically to
`quest::Parameters`. This object can express a URL-encoded form sent in the body
of the HTTP request. It can be constructed from `quest::Field` objects, another
alias for `std::pair<std::string, std::string>`. Similar to `quest::Parameters`,
construction can take adavantage of initiliazer lists as shown below:

{% raw %}

```c++
quest::SimpleForm form{{"key1", "value1"}, {"key2", "key2"}};

// Add additional field
quest::Parameter new_field{"key3", "value3"};
params.AddField(new_field);

auto res = quest::Post(quest::Url("https://httpbin.org/post"), form);
```

{% endraw %}

### Multipart

BeastQuest has support for [multipart requests](https://www.w3.org/Protocols/rfc1341/7_2_Multipart.html)
via the `quest::Multipart` class. The class allows files and form data to be
sent in a single request. `quest::Multipart` is a container for `quest::Parts`,
which can either be a pair of `std::string`s or a `std::string` and a
`quest::File`. Usage including intializer list construction is shown below:

{% raw %}

```c++
quest::Multipart multi{{"key", "value"}, {"file", quest::File("path-to-file")}};

// You can specify an optional MIME type for a quest::Part
quest::Part new_part("another file", quest::File("photo.jpg"), "image/jpeg");
multi.AddPart(new_part);

auto res = quest::Post(quest::Url("https://httpbin.org/post"), multi);
```

{% endraw %}
