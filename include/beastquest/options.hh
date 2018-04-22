#ifndef BEASTQUEST_OPTIONS_hH
#define BEASTQUEST_OPTIONS_hH

#include "beastquest/util.hh"

#include <map>
#include <string>

namespace quest {
// Type abstractions for common types
using Headers = std::map<std::string, std::string, CaseInsensitiveCompare>;

}  // namespace quest

#endif  // BEASTQUEST_OPTIONS_hH