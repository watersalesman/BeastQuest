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

void Parameters::AddParameter(Parameter parameter) noexcept {
  if (!content.empty()) {
    content += "&";
  }

  auto escapedKey = detail::UrlEncode(std::move(parameter.first));
  if (parameter.second.empty()) {
    content += std::move(escapedKey);
  } else {
    auto escapedValue = detail::UrlEncode(std::move(parameter.second));
    content += std::move(escapedKey) + "=" + std::move(escapedValue);
  }
}

}  // namespace quest