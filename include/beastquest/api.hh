#ifndef BEASTQUEST_API_HH
#define BEASTQUEST_API_HH

#include "beastquest/options.hh"
#include "beastquest/response.hh"
#include "beastquest/session.hh"

#include <future>

namespace quest {
// Synchronous request API
template <typename... Ts>
Response Get(Ts&&... ts) {
  Session sess;
  detail::SetOption(sess, std::forward<Ts>(ts)...);
  return sess.Get();
}

template <typename... Ts>
Response Post(Ts&&... ts) {
  Session sess;
  detail::SetOption(sess, std::forward<Ts>(ts)...);
  return sess.Post();
}

template <typename... Ts>
Response Delete(Ts&&... ts) {
  Session sess;
  detail::SetOption(sess, std::forward<Ts>(ts)...);
  return sess.Delete();
}

template <typename... Ts>
Response Put(Ts&&... ts) {
  Session sess;
  detail::SetOption(sess, std::forward<Ts>(ts)...);
  return sess.Put();
}

template <typename... Ts>
Response Patch(Ts&&... ts) {
  Session sess;
  detail::SetOption(sess, std::forward<Ts>(ts)...);
  return sess.Patch();
}

// Asynchronous request API
using AsyncResponse = std::future<Response>;

template <typename... Ts>
AsyncResponse AsyncGet(Ts&&... ts) {
  return std::async(std::launch::async,
                    [](Ts... ts) { return Get(std::forward<Ts>(ts)...); },
                    std::forward<Ts>(ts)...);
}

template <typename... Ts>
AsyncResponse AsyncPost(Ts&&... ts) {
  return std::async(std::launch::async,
                    [](Ts... ts) { return Post(std::forward<Ts>(ts)...); },
                    std::forward<Ts>(ts)...);
}

template <typename... Ts>
AsyncResponse AsyncDelete(Ts&&... ts) {
  return std::async(std::launch::async,
                    [](Ts... ts) { return Delete(std::forward<Ts>(ts)...); },
                    std::forward<Ts>(ts)...);
}

template <typename... Ts>
AsyncResponse AsyncPut(Ts&&... ts) {
  return std::async(std::launch::async,
                    [](Ts... ts) { return Put(std::forward<Ts>(ts)...); },
                    std::forward<Ts>(ts)...);
}

template <typename... Ts>
AsyncResponse AsyncPatch(Ts&&... ts) {
  return std::async(std::launch::async,
                    [](Ts... ts) { return Patch(std::forward<Ts>(ts)...); },
                    std::forward<Ts>(ts)...);
}

}  // namespace quest

#endif  // BEASTQUEST_API_HH