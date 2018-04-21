#ifndef BEASTQUEST_API_HH
#define BEASTQUEST_API_HH

#include "beastquest/auth.hh"
#include "beastquest/multipart.hh"
#include "beastquest/response.hh"
#include "beastquest/session.hh"
#include "beastquest/simpleform.hh"
#include "beastquest/url.hh"

#include <future>
#include <type_traits>

namespace quest {

namespace {
// Enables if types are the same without references and cv-qualifiers
template <typename T, typename U>
using enableIfSameClass_t = typename std::enable_if<
    std::is_same<T, typename std::decay<U>::type>::value, bool>::type;

// Template overloads for different Session options
template <typename T, enableIfSameClass_t<Url, T> = false>
void setOption(Session& session, T&& opt) {
  session.SetUrl(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<Headers, T> = false>
void setOption(Session& session, T&& opt) {
  session.SetHeaders(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<Parameters, T> = false>
void setOption(Session& session, T&& opt) {
  session.SetParameters(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<SimpleForm, T> = false>
void setOption(Session& session, T&& opt) {
  session.SetSimpleForm(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<Multipart, T> = false>
void setOption(Session& session, T&& opt) {
  session.SetMultipart(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<BasicAuth, T> = false>
void setOption(Session& session, T&& opt) {
  session.SetAuth(std::forward<T>(opt));
}

// If multiple options are given, set one option, then move on to the next one
template <typename T, typename... Ts>
void setOption(Session& session, T&& opt, Ts&&... ts) {
  setOption(session, std::forward<T>(opt));
  setOption(session, std::forward<Ts>(ts)...);
}

}  // namespace

// Synchronous request API
template <typename... Ts>
Response Get(Ts&&... ts) {
  Session sess;
  setOption(sess, std::forward<Ts>(ts)...);
  return sess.Get();
}

template <typename... Ts>
Response Post(Ts&&... ts) {
  Session sess;
  setOption(sess, std::forward<Ts>(ts)...);
  return sess.Post();
}

template <typename... Ts>
Response Delete(Ts&&... ts) {
  Session sess;
  setOption(sess, std::forward<Ts>(ts)...);
  return sess.Delete();
}

template <typename... Ts>
Response Put(Ts&&... ts) {
  Session sess;
  setOption(sess, std::forward<Ts>(ts)...);
  return sess.Put();
}

template <typename... Ts>
Response Patch(Ts&&... ts) {
  Session sess;
  setOption(sess, std::forward<Ts>(ts)...);
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