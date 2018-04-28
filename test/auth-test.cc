#include <gtest/gtest.h>
#include <beastquest/auth.hh>
#include <beastquest/session.hh>
#include <json.hpp>

#ifdef BeastQuest_LOCAL_HTTPBIN
std::string hb_server = "http://127.0.0.1:8000";
#endif
#ifndef BeastQuest_LOCAL_HTTPBIN
std::string hb_server = "http://httpbin.org";
#endif

using json = nlohmann::json;

TEST(AuthTest, AuthEmpty) {
  quest::BasicAuth auth;
  EXPECT_TRUE(auth.IsEmpty());
  EXPECT_TRUE(auth.GetAuthString().empty());
}

TEST(AuthTest, AuthRequest) {
  quest::BasicAuth auth("user", "pass");
  ASSERT_FALSE(auth.IsEmpty());
  ASSERT_FALSE(auth.GetAuthString().empty());

  quest::Session http;
  http.SetUrl(quest::Url(hb_server + "/basic-auth/user/pass"));
  http.SetAuth(auth);

  auto res = http.Get();
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  EXPECT_TRUE(res_json["authenticated"] == true);
}