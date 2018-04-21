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
  Response Get();
  Response Post();
  Response Delete();
  Response Put();
  Response Patch();

 private:
  class Impl;
  std::unique_ptr<Impl> pimpl_;
};

}  // namespace quest

#endif  // BEASTQUEST_SESSION_HH