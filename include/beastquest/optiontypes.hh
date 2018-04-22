#ifndef BEASTQUEST_OPTIONTYPES_HH
#define BEASTQUEST_OPTIONTYPES_HH

#include "beastquest/util.hh"

#include <map>
#include <string>

namespace quest {
// Type abstractions for common types
using Headers = std::map<std::string, std::string, detail::CaseInsensitiveCompare>;

struct Body {
  Body() = default;
  Body(std::string body) : content(std::move(body)) {}
  std::string content;
};

}  // namespace quest

#endif  // BEASTQUEST_OPTIONTYPES_HH