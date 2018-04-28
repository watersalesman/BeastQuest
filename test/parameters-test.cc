#include <gtest/gtest.h>
#include <beastquest/parameters.hh>
#include <beastquest/session.hh>
#include <json.hpp>

#ifdef BeastQuest_LOCAL_HTTPBIN
std::string hb_server = "http://127.0.0.1:8000";
#endif
#ifndef BeastQuest_LOCAL_HTTPBIN
std::string hb_server = "http://httpbin.org";
#endif

using json = nlohmann::json;

TEST(ParamTest, ParamContruction) {
  quest::Parameters params = {{"k1", "v1"}, {"k2", "v2"}};
  ASSERT_EQ(params.content, "k1=v1&k2=v2");
}

TEST(ParamTest, AddParam) {
  quest::Parameters params;
  params.AddParameter(quest::Parameter("k1", "v1"));
  ASSERT_EQ(params.content, "k1=v1");
}

TEST(ParamTest, ParamRequest) {
  quest::Parameters params = {{"k1", "v1"}, {"k2", "v2"}};

  quest::Session http;
  http.SetUrl(quest::Url(hb_server + "/get"));
  http.SetParameters(params);

  auto res = http.Get();
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  auto args = res_json["args"];
  ASSERT_FALSE(args.empty());
  EXPECT_TRUE(args["k1"] == "v1");
  EXPECT_TRUE(args["k2"] == "v2");
}