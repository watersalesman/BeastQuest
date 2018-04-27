# Session

The session-level API is implemented in the `quest::Session` class.

It allows you to easily retain settings between requests. Similar to the
requests API, the only required setting is a `quest::Url` object. Settings are
changed via an assortment of setter functions, using the naming convention of
`Session::Set<Setting>`. You can see the up-to-date list of functions and their
parameters in the [session.hh](https://github.com/watersalesman/BeastQuest/blob/master/include/beastquest/session.hh)
header file.

`quest::Session` has member functions for each request type as shown below:

```c++
quest::Session sess;
quest::Response res;

sess.SetUrl(quest::Url("https://httpbin.org/get"));
res = sess.Get();

sess.SetUrl(quest::Url("https://httpbin.org/post"));
res = sess.Post();

sess.SetUrl(quest::Url("https://httpbin.org/delete"));
res = sess.Delete();

sess.SetUrl(quest::Url("https://httpbin.org/put"));
res = sess.Put();

sess.SetUrl(quest::Url("https://httpbin.org/patch"));
res = sess.Patch();
```