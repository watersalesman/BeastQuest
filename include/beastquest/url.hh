#ifndef BEASTQUEST_URL_HH
#define BEASTQUEST_URL_HH

#include <string>

namespace quest {

const std::string DEFAULT_HTTP_PORT("80");
const std::string DEFAULT_HTTPS_PORT("443");

struct Url {
  Url() = default;
  Url(const std::string&);
  std::string host;
  std::string path;
  std::string port;
  bool use_ssl;
  bool CanShareConnectionWith(const Url&) const noexcept;
};

}  // namespace quest

#endif  // BEASTQUEST_URL_HH