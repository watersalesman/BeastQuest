#ifndef BEASTQUEST_AUTH_HH
#define BEASTQUEST_AUTH_HH

#include <string>

namespace quest {

class BasicAuth {
 public:
  BasicAuth() = default;
  BasicAuth(std::string username, std::string password);
  std::string GetAuthString() const noexcept;
  bool IsEmpty() const noexcept;

 private:
  std::string username_;
  std::string password_;
  std::string auth_string_;
};

}  // namespace quest

#endif  // BEASTQUEST_AUTH_HH