#include "beastquest/url.hh"

#include <regex>

namespace quest {

Url::Url(const std::string& url) : use_ssl(false) {
  std::regex re_pattern(
      "^\\s*(http[s]?)://([A-Za-z0-9\\-\\.]+)(:[0-9]+)?([[\\/?]+.*)?\\s*$");
  std::smatch re_capture;
  std::regex_search(url, re_capture, re_pattern);

  // A successful capture should return five matches, even if some are empty
  if (re_capture.size() != 5)
    throw std::runtime_error("quest::Url - Failed to parse invalid URL");

  host = re_capture[2];
  path = re_capture[4].length() ? static_cast<std::string>(re_capture[4]) : "/";
  use_ssl = re_capture[1] == "https";

  // Determine port number
  if (re_capture[3].length()) {
    port = re_capture[3];
    port = port.substr(1);
  } else if (use_ssl)
    port = DEFAULT_HTTPS_PORT;
  else
    port = DEFAULT_HTTP_PORT;
}

bool Url::CanShareConnectionWith(const Url& rhs) const noexcept {
  // Do not attempt to share sockets when communicating with a local server
  if (rhs.host == "127.0.0.1" || rhs.host == "localhost") return false;

  if (this->host == rhs.host && this->port == rhs.port &&
      this->use_ssl == rhs.use_ssl)
    return true;
  else
    return false;
}

}  // namespace quest