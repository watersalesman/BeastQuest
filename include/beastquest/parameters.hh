#ifndef BEASTQUEST_PARAM_HH
#define BEASTQUEST_PARAM_HH

#include <initializer_list>
#include <string>
#include <utility>

namespace quest {

using Parameter = std::pair<std::string, std::string>;

struct Parameters {
 public:
  Parameters() = default;
  Parameters(const std::initializer_list<Parameter>& parameters);

  void AddParameter(const Parameter& parameter) noexcept;

  std::string content;
};

}  // namespace quest

#endif  // BEASTQUEST_PARAM_HH