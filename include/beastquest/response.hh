#ifndef BEASTQUEST_RESPONSE_HH
#define BEASTQUEST_RESPONSE_HH

#include "beastquest/util.hh"

#include <map>
#include <string>

namespace quest {

using Headers = std::map<std::string, std::string, CaseInsensitiveCompare>;

struct Response {
  Headers headers;
  std::string content;
  int status_code = 0;
  std::string reason;
};

}  // namespace quest

#endif  // BEASTQUEST_RESPONSE_HH