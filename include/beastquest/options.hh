#ifndef BEASTQUEST_OPTIONS_HH
#define BEASTQUEST_OPTIONS_HH

#include "beastquest/auth.hh"
#include "beastquest/multipart.hh"
#include "beastquest/optiontypes.hh"
#include "beastquest/parameters.hh"
#include "beastquest/session.hh"
#include "beastquest/simpleform.hh"
#include "beastquest/url.hh"
#include "beastquest/util.hh"

#include <map>
#include <string>
#include <type_traits>

namespace quest {
namespace detail {

// Type alias that enables if type parameters are the same without references
// and cv-qualifiers
template <typename T, typename U>
using enableIfSameClass_t = typename std::enable_if<
    std::is_same<T, typename std::decay<U>::type>::value, bool>::type;

// Template overloads for different Session options
template <typename T, enableIfSameClass_t<Url, T> = false>
void SetOption(Session& sess, T&& opt) {
  sess.SetUrl(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<Headers, T> = false>
void SetOption(Session& sess, T&& opt) {
  sess.SetHeaders(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<Body, T> = false>
void SetOption(Session& sess, T&& opt) {
  sess.SetBody(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<Parameters, T> = false>
void SetOption(Session& sess, T&& opt) {
  sess.SetParameters(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<SimpleForm, T> = false>
void SetOption(Session& sess, T&& opt) {
  sess.SetSimpleForm(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<Multipart, T> = false>
void SetOption(Session& sess, T&& opt) {
  sess.SetMultipart(std::forward<T>(opt));
}

template <typename T, enableIfSameClass_t<BasicAuth, T> = false>
void SetOption(Session& sess, T&& opt) {
  sess.SetAuth(std::forward<T>(opt));
}

// If multiple options are given, set one option, then move on to the next one
template <typename T, typename... Ts>
void SetOption(Session& sess, T&& opt, Ts&&... ts) {
  SetOption(sess, std::forward<T>(opt));
  SetOption(sess, std::forward<Ts>(ts)...);
}

}  // namespace detail
}  // namespace quest

#endif  // BEASTQUEST_OPTIONS_HH