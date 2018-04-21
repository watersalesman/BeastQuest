#include "beastquest/multipart.hh"

#include "beastquest/util.hh"

#include <fstream>
#include <initializer_list>
#include <string>
#include <vector>

namespace quest {

void Multipart::AddPart(Part part) { parts_.emplace_back(std::move(part)); }

int Multipart::numParts() { return parts_.size(); }

std::string Multipart::MimeForm() const {
  std::string mime_form;

  if (parts_.size() == 0) return mime_form;

  std::string prefix = "--";
  std::string newline = "\r\n";

  for (const Part& part : parts_) {
    mime_form += prefix + boundary_;
    mime_form += newline;
    mime_form += "Content-Disposition: form-data; name=\"" + part.name + "\"";
    if (part.is_file) {
      mime_form += "; filename=\"" + part.name + "\"";
      mime_form += newline;
      mime_form += "Content-Type: " + part.content_type;
    }
    mime_form += newline + newline;
    if (part.is_file) {
      std::ifstream file_stream(part.value, std::ios::binary);
      mime_form += std::string(std::istreambuf_iterator<char>(file_stream),
                               std::istreambuf_iterator<char>());
    } else {
      mime_form += part.value;
    }
    mime_form += newline;
  }

  mime_form += prefix + boundary_ + prefix + newline;

  return mime_form;
}

std::string& Multipart::ContentType() noexcept { return content_type_; }

}  // namespace quest