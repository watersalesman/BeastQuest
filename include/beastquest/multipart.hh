#ifndef BEASTQUEST_MULTIPART_HH
#define BEASTQUEST_MULTIPART_HH

#include "beastquest/util.hh"

#include <initializer_list>
#include <string>
#include <vector>

namespace quest {

struct File {
  File(std::string filepath) : filepath(std::move(filepath)) {}
  std::string filepath;
};

struct Part {
  Part(std::string name, std::string value, std::string content_type = "")
      : name(std::move(name)),
        value(std::move(value)),
        content_type(content_type.empty() ? "text/plain"
                                          : std::move(content_type)),
        is_file(false) {}
  Part(std::string name, const File& file, std::string content_type = "")
      : name(std::move(name)),
        value(file.filepath),
        content_type(content_type.empty() ? "text/plain"
                                          : std::move(content_type)),
        is_file(true) {}

  std::string name;
  std::string value;
  std::string content_type;
  bool is_file;
};

class Multipart {
 public:
  Multipart() = default;
  Multipart(const std::initializer_list<Part>& parts)
      : parts_(parts),
        boundary_("-----------simple-boundary"),
        content_type_("multipart/form-data; boundary=" + boundary_) {}
  void AddPart(Part part);
  int numParts();
  std::string MimeForm() const;
  std::string& ContentType() noexcept;

 private:
  std::vector<Part> parts_;
  std::string boundary_;
  std::string content_type_;
};

}  // namespace quest

#endif  // BEASTQUEST_MULTIPART_HH