#ifndef BEASTQUEST_SIMPLEFORM_HH
#define BEASTQUEST_SIMPLEFORM_HH

#include <initializer_list>
#include <string>
#include <utility>

namespace quest {

using Field = std::pair<std::string, std::string>;

struct SimpleForm {
 public:
  SimpleForm() = default;
  SimpleForm(const std::initializer_list<Field>& fields);

  void AddField(const Field& field) noexcept;

  std::string content;
};

}  // namespace quest

#endif  // BEASTQUEST_SIMPLEFORM_HH