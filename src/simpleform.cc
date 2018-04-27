#include "beastquest/simpleform.hh"

#include <initializer_list>
#include <string>
#include <utility>

namespace quest {

SimpleForm::SimpleForm(const std::initializer_list<Field>& fields) {
  for (const auto& field : fields) {
    AddField(field);
  }
}

void SimpleForm::AddField(Field field) noexcept {
  if (!content.empty()) {
    content += "&";
  }

  if (field.second.empty()) {
    content += std::move(field.first);
  } else {
    content += std::move(field.first) + "=" + std::move(field.second);
  }
}

}  // namespace quest