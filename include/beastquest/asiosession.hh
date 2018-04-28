#ifndef BEASTQUEST_ASIOSESSION_HH
#define BEASTQUEST_ASIOSESSION_HH

#include "beastquest/url.hh"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

namespace http = boost::beast::http;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;
using BeastResponse = http::response<http::string_body>;
using Request = http::request<http::string_body>;

namespace quest {
namespace detail {

class AsioSession {
 public:
  AsioSession();
  quest::Url& Url();
  bool HasOpenSocket();
  void Connect(const quest::Url& url);
  void Write(Request& req);
  void Read(BeastResponse& req);
  void CloseSocket();

 private:
  boost::asio::io_context ioc_;
  tcp::resolver resolver_{ioc_};
  tcp::socket socket_{ioc_};
  ssl::context ctx_{ssl::context::tlsv12_client};
  std::unique_ptr<ssl::stream<tcp::socket>> ssl_stream_;
  quest::Url url_;
};

AsioSession::AsioSession() {
  ctx_.set_default_verify_paths();
  ssl_stream_.reset(new ssl::stream<tcp::socket>{ioc_, ctx_});
}

quest::Url& AsioSession::Url() { return url_; }

bool AsioSession::HasOpenSocket() {
  return socket_.is_open() || ssl_stream_->next_layer().is_open();
}

void AsioSession::Connect(const quest::Url& url) {
  // Resolve using host
  auto const results = resolver_.resolve(url.host, url.port);

  if (url.use_ssl) {
    // Set SNI Hostname (many hosts need this to handshake successfully)
    if (!SSL_set_tlsext_host_name(ssl_stream_->native_handle(),
                                  url.host.c_str())) {
      boost::system::error_code ec{static_cast<int>(::ERR_get_error()),
                                   boost::asio::error::get_ssl_category()};
      throw boost::system::system_error{ec};
    }

    // Connect and perfom SSL handshake
    boost::asio::connect(ssl_stream_->next_layer(), results.begin(),
                         results.end());
    ssl_stream_->handshake(ssl::stream_base::client);
  } else {
    boost::asio::connect(socket_, results.begin(), results.end());
  }
}

void AsioSession::Write(Request& req) {
  if (url_.use_ssl) {
    http::write(*ssl_stream_, req);
  } else
    http::write(socket_, req);
}

void AsioSession::Read(BeastResponse& res) {
  boost::beast::flat_buffer buffer;
  if (url_.use_ssl)
    http::read(*ssl_stream_, buffer, res);
  else
    http::read(socket_, buffer, res);
}

void AsioSession::CloseSocket() {
  boost::system::error_code ec;
  if (socket_.is_open()) {
    socket_.shutdown(tcp::socket::shutdown_both, ec);
    socket_.close();
  }
  if (ssl_stream_->next_layer().is_open()) {
    // SSL streams cannot be shared between different hosts and must be
    // recreated
    ssl_stream_->next_layer().shutdown(tcp::socket::shutdown_both, ec);
    ssl_stream_->next_layer().close();
    ssl_stream_.reset(new ssl::stream<tcp::socket>{ioc_, ctx_});
  }
}

}  // namespace detail
}  // namespace quest

#endif  // BEASTQUEST_ASIOSESSION_HH