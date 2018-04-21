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

void SimpleForm::AddField(const Field& field) noexcept {
  if (!content.empty()) {
    content += "&";
  }

  if (field.second.empty()) {
    content += field.first;
  } else {
    content += field.first + "=" + field.second;
  }
}

}  // namespace quest