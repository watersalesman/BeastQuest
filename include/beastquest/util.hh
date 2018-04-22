#ifndef BEASTQUEST_UTIL_HH
#define BEASTQUEST_UTIL_HH

#include <string>

namespace quest {
namespace detail {

template <typename target, typename source>
void AssertIsContructible() {
  static_assert(std::is_constructible<target, source>::value,
                "Cannot construct necessary type from parameter.");
}

std::string UrlEncode(const std::string& value);

struct CaseInsensitiveCompare {
  bool operator()(std::string lhs, std::string rhs) const noexcept;
};

}  // namespace detail
}  // namespace quest

#endif  // BEASTQUEST_UTIL_HH