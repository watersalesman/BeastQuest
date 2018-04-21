#include "beastquest/auth.hh"

#include <boost/beast.hpp>
#include <string>

namespace detail = boost::beast::detail;
namespace quest {

BasicAuth::BasicAuth(std::string username, std::string password)
    : username_{std::move(username)},
      password_{std::move(password)},
      auth_string_("Basic " +
                   detail::base64_encode(username_ + ":" + password_)) {}

std::string BasicAuth::GetAuthString() const noexcept { return auth_string_; }

bool BasicAuth::IsEmpty() const noexcept { return auth_string_.empty(); }

}  // namespace quest