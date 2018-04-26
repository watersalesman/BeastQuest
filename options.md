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

Coming soon...

### Url
### Headers
### Body
### BasicAuth
### Parameters
### SimpleForm
### Multipart