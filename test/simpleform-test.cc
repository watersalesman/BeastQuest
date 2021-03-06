#include <gtest/gtest.h>
#include <beastquest/session.hh>
#include <beastquest/simpleform.hh>
#include <json.hpp>

#ifdef BeastQuest_LOCAL_HTTPBIN
std::string hb_server = "http://127.0.0.1:8000";
#endif
#ifndef BeastQuest_LOCAL_HTTPBIN
std::string hb_server = "http://httpbin.org";
#endif

using json = nlohmann::json;

TEST(SimpleFormTest, FormRequest) {
  quest::Session sess;
  sess.SetUrl(quest::Url(hb_server + "/post"));
  sess.SetSimpleForm(quest::SimpleForm({{"k1", "v1"}, {"k2", "v2"}}));

  auto res = sess.Post();
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  auto form = res_json["form"];
  EXPECT_TRUE(form["k1"] == "v1");
  EXPECT_TRUE(form["k2"] == "v2");
}