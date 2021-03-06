#ifndef BEASTQUEST_OPTIONTYPES_HH
#define BEASTQUEST_OPTIONTYPES_HH

#include "beastquest/util.hh"

#include <map>
#include <string>

namespace quest {
// Type abstractions for common types
using Headers =
    std::map<std::string, std::string, detail::CaseInsensitiveCompare>;

struct Body {
  Body() = default;
  Body(std::string body) : content(std::move(body)) {}
  std::string content;
};

struct VerifySSL {
  VerifySSL() = default;
  VerifySSL(int val) : value(val) {}
  bool value;
};

struct MaxRedirects {
  MaxRedirects() = default;
  MaxRedirects(int val) : value(val) {}
  int value;
};

struct UserAgent {
  UserAgent() = default;
  UserAgent(std::string user_agent) : content(std::move(user_agent)) {}
  std::string content;
};

}  // namespace quest

#endif  // BEASTQUEST_OPTIONTYPES_HH