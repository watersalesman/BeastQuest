#include "beastquest/session.hh"

#include "beastquest/asiosession.hh"
#include "beastquest/auth.hh"
#include "beastquest/multipart.hh"
#include "beastquest/optiontypes.hh"
#include "beastquest/parameters.hh"
#include "beastquest/response.hh"
#include "beastquest/simpleform.hh"
#include "beastquest/url.hh"
#include "beastquest/util.hh"

#include <boost/beast.hpp>
#include <memory>
#include <string>

namespace http = boost::beast::http;
using Request = http::request<http::string_body>;
using Method = http::verb;
using BeastResponse = http::response<http::string_body>;

namespace {

// Construct Response object from BeastResponse
quest::Response MakeResponse(BeastResponse&& beast_res) {
  quest::Response res;

  res.content = std::move(beast_res.body());
  res.status_code = beast_res.result_int();
  res.reason =
      std::string(beast_res.reason().cbegin(), beast_res.reason().cend());

  for (const auto& header : beast_res.base()) {
    auto key =
        std::string(header.name_string().cbegin(), header.name_string().cend());
    auto value = std::string(header.value().cbegin(), header.value().cend());
    res.headers.insert({key, value});
  }

  return res;
}

}  // namespace

namespace quest {
using namespace detail;

class Session::Impl {
 public:
  Impl()
      : version_(11),
        user_agent_(BOOST_BEAST_VERSION_STRING),
        keep_alive_(true),
        max_redirects_(-1) {}
  void SetKeepAlive(bool keep_alive) noexcept;
  void SetVerifySSL(bool verify_ssl_) noexcept;
  void SetMaxRedirects(int max_redirects) noexcept;
  void SetUserAgent(std::string user_agent) noexcept;
  template <typename T>
  void SetUrl(T&& url) noexcept;
  template <typename T>
  void SetHeaders(T&& headers) noexcept;
  template <typename T>
  void SetBody(T&& body) noexcept;
  template <typename T>
  void SetParameters(T&& parameters) noexcept;
  template <typename T>
  void SetSimpleForm(T&& form) noexcept;
  template <typename T>
  void SetMultipart(T&& multipart) noexcept;
  template <typename T>
  void SetAuth(T&& basic_auth) noexcept;
  BeastResponse Get();
  BeastResponse Post();
  BeastResponse Delete();
  BeastResponse Put();
  BeastResponse Patch();

 private:
  AsioSession asio_;
  int version_;
  std::string user_agent_;
  bool keep_alive_;
  int max_redirects_;
  Headers headers_;
  Body body_;
  Parameters parameters_;
  SimpleForm simple_form_;
  Multipart multipart_;
  BasicAuth basic_auth_;
  Request createRequest(const Url& url, const Method& method);
  BeastResponse sendRequest(AsioSession& asio_session, const Method& method,
                            bool is_redirect = false);
  void followRedirect(BeastResponse& res, const Method& method);
};

template <typename T>
void Session::Impl::SetUrl(T&& url) noexcept {
  detail::AssertIsContructible<Url, T>();
  // Close any active connection if switching session to a new host
  if (asio_.HasOpenSocket() && !asio_.Url().CanShareConnectionWith(url)) {
    asio_.CloseSocket();
  }
  asio_.Url() = std::forward<T>(url);
}

template <typename T>
void Session::Impl::SetHeaders(T&& headers) noexcept {
  detail::AssertIsContructible<Headers, T>();
  headers_ = std::forward<T>(headers);
}

template <typename T>
void Session::Impl::SetBody(T&& body) noexcept {
  detail::AssertIsContructible<Body, T>();
  body_ = std::forward<T>(body);
}

template <typename T>
void Session::Impl::SetParameters(T&& parameters) noexcept {
  detail::AssertIsContructible<Parameters, T>();
  parameters_ = std::forward<T>(parameters);
}

template <typename T>
void Session::Impl::SetSimpleForm(T&& form) noexcept {
  detail::AssertIsContructible<SimpleForm, T>();
  simple_form_ = std::forward<T>(form);
}

template <typename T>
void Session::Impl::SetMultipart(T&& multipart) noexcept {
  detail::AssertIsContructible<Multipart, T>();
  multipart_ = std::forward<T>(multipart);
}

template <typename T>
void Session::Impl::SetAuth(T&& basic_auth) noexcept {
  detail::AssertIsContructible<BasicAuth, T>();
  basic_auth_ = std::forward<T>(basic_auth);
}

void Session::Impl::SetKeepAlive(bool keep_alive) noexcept {
  keep_alive_ = keep_alive;
}

void Session::Impl::SetVerifySSL(bool verify_ssl) noexcept {
  asio_.SetVerifySSL(verify_ssl);
}

void Session::Impl::SetMaxRedirects(int max_redirects) noexcept {
  max_redirects_ = max_redirects;
}

void Session::Impl::SetUserAgent(std::string user_agent) noexcept {
  user_agent_ = std::move(user_agent);
}

BeastResponse Session::Impl::Get() { return sendRequest(asio_, Method::get); }

BeastResponse Session::Impl::Post() { return sendRequest(asio_, Method::post); }

BeastResponse Session::Impl::Delete() {
  return sendRequest(asio_, Method::delete_);
}

BeastResponse Session::Impl::Put() { return sendRequest(asio_, Method::put); }

BeastResponse Session::Impl::Patch() {
  return sendRequest(asio_, Method::patch);
}

Request Session::Impl::createRequest(const Url& url, const Method& method) {
  // Set URL path, including parameters
  std::string path = url.path;
  if (!parameters_.content.empty()) {
    if (path.find("?") == std::string::npos)
      path += "?";
    else
      path += "&";
    path += parameters_.content;
  }

  // Create request object
  Request req{method, path, version_};

  // Set custom body, Multipart, or SimpleForm
  if (!body_.content.empty()) {
    req.body() = body_.content;
  } else if (multipart_.numParts()) {
    req.set(http::field::content_type, multipart_.ContentType());
    req.body() = multipart_.MimeForm();
  } else if (!simple_form_.content.empty()) {
    req.set(http::field::content_type, "application/x-www-form-urlencoded");
    req.body() = simple_form_.content;
  }

  // Set standard headers
  req.set(http::field::host, url.host);
  req.set(http::field::user_agent, user_agent_);
  req.keep_alive(keep_alive_);

  // Set Basic Auth
  if (!basic_auth_.IsEmpty())
    req.set(http::field::authorization, basic_auth_.GetAuthString());

  // Set custom headers
  if (!headers_.empty()) {
    for (const auto& header_pair : headers_)
      req.set(header_pair.first, header_pair.second);
  }

  // Prepare headers based on body contents
  req.prepare_payload();

  return req;
}

BeastResponse Session::Impl::sendRequest(AsioSession& asio_session,
                                         const Method& method,
                                         bool is_redirect) {
  Url& url = asio_session.Url();

  // Set up an HTTP request message
  auto req = createRequest(url, method);

  // Connect if socket is closed
  if (!asio_session.HasOpenSocket()) {
    asio_session.Connect(url);
  }

  // Send the HTTP request to the remote host
  asio_session.Write(req);

  // Receive the HTTP response
  BeastResponse res;
  asio_session.Read(res);

  // Close connection if needed
  if (!keep_alive_ && asio_session.HasOpenSocket()) {
    asio_session.CloseSocket();
  }

  // Follow redirects up until max redirects (default infinity)
  if ((300 <= res.result_int() && res.result_int() <= 399) && !is_redirect) {
    followRedirect(res, method);
  }

  return res;
}

void Session::Impl::followRedirect(BeastResponse& res, const Method& method) {
  // Make copy of Session Url to reassign at the end of the redirects if needed
  Url session_url = asio_.Url();
  AsioSession redirect_asio;
  redirect_asio.Url() = session_url;
  Url& redirect_url = redirect_asio.Url();

  /* This will loop until either the response status code is not 3xx or until
   * the number of max redirects is reached. A max redirects_ of -1 means to
   * follow infinite redirects, meaning "int i" will not increment in that
   * case. */
  for (int i = 0; 300 <= res.result_int() && res.result_int() <= 399 &&
                  (i < max_redirects_ || max_redirects_ == -1);
       i += max_redirects_ < 0 ? 0 : 1) {
    // Get new location from redirect response
    std::string location = res.at("Location").data();

    // Don't bother with a bad redirect. Just break and return
    if (location.empty())
      break;
    else {
      // Remove whitespace at the end of header value
      auto ws_pos = location.find("\r\n");
      if (ws_pos != std::string::npos)
        location.erase(ws_pos, location.size() - 1);
    }

    // Determine if redirect is absolute or relative location
    try {
      redirect_url = Url(location);
    } catch (const std::runtime_error& e) {
      redirect_url.path = location;
    }

    // If Keep-Alive is set, attempt to reuse either the session socket or
    // redirect socket
    if (keep_alive_ && session_url.CanShareConnectionWith(redirect_url)) {
      asio_.Url() = redirect_url;
      res = sendRequest(asio_, method, true);
    } else {
      if ((!redirect_url.CanShareConnectionWith(session_url) || !keep_alive_) &&
          redirect_asio.HasOpenSocket()) {
        redirect_asio.CloseSocket();
      }
      res = sendRequest(redirect_asio, method, true);
    }
  }
  // Reassign original Url object to member AsioSession in case it was changed
  asio_.Url() = session_url;
}

Session::Session() : pimpl_(new Impl()) {}
Session::~Session() {}

void Session::SetKeepAlive(bool keep_alive) noexcept {
  pimpl_->SetKeepAlive(keep_alive);
}

void Session::SetVerifySSL(bool verify_ssl) noexcept {
  pimpl_->SetVerifySSL(verify_ssl);
}

void Session::SetMaxRedirects(int max_redirects) noexcept {
  pimpl_->SetMaxRedirects(max_redirects);
}

void Session::SetUserAgent(std::string user_agent) noexcept {
  pimpl_->SetUserAgent(user_agent);
}

void Session::SetUrl(const Url& url) noexcept { pimpl_->SetUrl(url); }

void Session::SetUrl(Url&& url) noexcept { pimpl_->SetUrl(std::move(url)); }

void Session::SetHeaders(const Headers& headers) noexcept {
  pimpl_->SetHeaders(headers);
}

void Session::SetHeaders(Headers&& headers) noexcept {
  pimpl_->SetHeaders(std::move(headers));
}

void Session::SetBody(const Body& body) noexcept { pimpl_->SetBody(body); }

void Session::SetBody(Body&& body) noexcept {
  pimpl_->SetBody(std::move(body));
}

void Session::SetParameters(const Parameters& parameters) noexcept {
  pimpl_->SetParameters(parameters);
}

void Session::SetParameters(Parameters&& parameters) noexcept {
  pimpl_->SetParameters(std::move(parameters));
}

void Session::SetSimpleForm(const SimpleForm& form) noexcept {
  pimpl_->SetSimpleForm(form);
}

void Session::SetSimpleForm(SimpleForm&& form) noexcept {
  pimpl_->SetSimpleForm(std::move(form));
}

void Session::SetMultipart(const Multipart& multipart) noexcept {
  pimpl_->SetMultipart(multipart);
}

void Session::SetMultipart(Multipart&& multipart) noexcept {
  pimpl_->SetMultipart(std::move(multipart));
}

void Session::SetAuth(const BasicAuth& basic_auth) noexcept {
  pimpl_->SetAuth(basic_auth);
}

void Session::SetAuth(BasicAuth&& basic_auth) noexcept {
  pimpl_->SetAuth(std::move(basic_auth));
}

Response Session::Get() { return MakeResponse(pimpl_->Get()); }
Response Session::Post() { return MakeResponse(pimpl_->Post()); }
Response Session::Delete() { return MakeResponse(pimpl_->Delete()); }
Response Session::Put() { return MakeResponse(pimpl_->Put()); }
Response Session::Patch() { return MakeResponse(pimpl_->Patch()); }

}  // namespace quest