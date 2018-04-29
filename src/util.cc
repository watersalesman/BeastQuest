#include "beastquest/util.hh"

#include <boost/algorithm/string.hpp>
#include <cctype>
#include <iomanip>
#include <sstream>

namespace quest {
namespace detail {

std::string UrlEncode(const std::string& value) {
  std::ostringstream escaped;
  escaped.fill('0');
  escaped << std::hex;

  for (auto i = value.cbegin(), n = value.cend(); i != n; ++i) {
    std::string::value_type c = (*i);
    // Keep alphanumeric and other accepted characters intact
    if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      escaped << c;
      continue;
    }
    // Any other characters are percent-encoded
    escaped << '%' << std::setw(2)
            << std::int32_t(static_cast<unsigned char>(c));
  }

  return escaped.str();
}

bool CaseInsensitiveCompare::operator()(std::string lhs, std::string rhs) const
    noexcept {
  boost::algorithm::to_lower(lhs);
  boost::algorithm::to_lower(rhs);
  return lhs < rhs;
}

}  // namespace detail
}  // namespace quest
