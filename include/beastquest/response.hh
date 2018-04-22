#ifndef BEASTQUEST_RESPONSE_HH
#define BEASTQUEST_RESPONSE_HH

#include "beastquest/optiontypes.hh"

#include <string>

namespace quest {

struct Response {
  Headers headers;
  std::string content;
  int status_code = 0;
  std::string reason;
};

}  // namespace quest

#endif  // BEASTQUEST_RESPONSE_HH