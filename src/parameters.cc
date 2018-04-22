#include "beastquest/parameters.hh"

#include "beastquest/util.hh"

#include <initializer_list>
#include <string>
#include <utility>

namespace quest {

Parameters::Parameters(const std::initializer_list<Parameter>& parameters) {
  for (const auto& parameter : parameters) {
    AddParameter(parameter);
  }
}

void Parameters::AddParameter(const Parameter& parameter) noexcept {
  if (!content.empty()) {
    content += "&";
  }

  auto escapedKey = detail::UrlEncode(parameter.first);
  if (parameter.second.empty()) {
    content += escapedKey;
  } else {
    auto escapedValue = detail::UrlEncode(parameter.second);
    content += escapedKey + "=" + escapedValue;
  }
}

}  // namespace quest