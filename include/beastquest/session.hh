#ifndef BEASTQUEST_SESSION_HH
#define BEASTQUEST_SESSION_HH

#include "beastquest/auth.hh"
#include "beastquest/multipart.hh"
#include "beastquest/parameters.hh"
#include "beastquest/response.hh"
#include "beastquest/simpleform.hh"
#include "beastquest/url.hh"

#include <memory>
#include <string>

namespace quest {

// Alias that enables if types are the same without references and cv-qualifiers
template <typename T, typename U>
using enableIfSameClass_t = typename std::enable_if<
    std::is_same<T, typename std::decay<U>::type>::value, bool>::type;

class Session {
 public:
  Session();
  ~Session();
  void SetKeepAlive(bool keep_alive) noexcept;
  void SetVerifySSL(bool verify_ssl) noexcept;
  void SetMaxRedirects(int max_redirects) noexcept;
  void SetUserAgent(std::string user_agent) noexcept;
  void SetUrl(const Url& url) noexcept;
  void SetUrl(Url&& url) noexcept;
  void SetHeaders(const Headers& headers) noexcept;
  void SetHeaders(Headers&& headers) noexcept;
  void SetParameters(const Parameters& parameters) noexcept;
  void SetParameters(Parameters&& parameters) noexcept;
  void SetSimpleForm(const SimpleForm& form) noexcept;
  void SetSimpleForm(SimpleForm&& form) noexcept;
  void SetMultipart(const Multipart& multipart) noexcept;
  void SetMultipart(Multipart&& multipart) noexcept;
  void SetAuth(const BasicAuth& basic_auth) noexcept;
  void SetAuth(BasicAuth&& basic_auth) noexcept;
  template <typename T, enableIfSameClass_t<Url, T> = false>
  void SetOption(T&& opt);
  template <typename T, enableIfSameClass_t<Headers, T> = false>
  void SetOption(T&& opt);
  template <typename T, enableIfSameClass_t<Parameters, T> = false>
  void SetOption(T&& opt);
  template <typename T, enableIfSameClass_t<SimpleForm, T> = false>
  void SetOption(T&& opt);
  template <typename T, enableIfSameClass_t<Multipart, T> = false>
  void SetOption(T&& opt);
  template <typename T, enableIfSameClass_t<BasicAuth, T> = false>
  void SetOption(T&& opt);
  template <typename T, typename... Ts>
  void SetOption(T&& opt, Ts&&... ts);
  Response Get();
  Response Post();
  Response Delete();
  Response Put();
  Response Patch();

 private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

// Template overloads for different Session options
template <typename T, enableIfSameClass_t<Url, T> = false>
void Session::SetOption(T&& opt) {
  SetUrl(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<Headers, T> = false>
void Session::SetOption(T&& opt) {
  SetHeaders(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<Parameters, T> = false>
void Session::SetOption(T&& opt) {
  SetParameters(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<SimpleForm, T> = false>
void Session::SetOption(T&& opt) {
  SetSimpleForm(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<Multipart, T> = false>
void Session::SetOption(T&& opt) {
  SetMultipart(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<BasicAuth, T> = false>
void Session::SetOption(T&& opt) {
  SetAuth(std::forward<T>(opt));
}

// If multiple options are given, set one option, then move on to the next one
template <typename T, typename... Ts>
void Session::SetOption(T&& opt, Ts&&... ts) {
  SetOption(std::forward<T>(opt));
  SetOption(std::forward<Ts>(ts)...);
}

}  // namespace quest

#endif  // BEASTQUEST_SESSION_HH