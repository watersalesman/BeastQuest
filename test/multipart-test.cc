#include <gtest/gtest.h>
#include <beastquest/multipart.hh>
#include <beastquest/session.hh>
#include <fstream>
#include <json.hpp>

#ifdef BeastQuest_LOCAL_HTTPBIN
std::string hb_server = "http://127.0.0.1:8000";
#endif
#ifndef BeastQuest_LOCAL_HTTPBIN
std::string hb_server = "http://httpbin.org";
#endif

using json = nlohmann::json;

TEST(MultipartTest, FormData) {
  quest::Session sess;
  sess.SetUrl(quest::Url(hb_server + "/post"));

  quest::Multipart mp = {{"k1", "v1"}};
  mp.AddPart(quest::Part("k2", "v2"));
  ASSERT_EQ(mp.numParts(), 2);
  sess.SetMultipart(mp);

  auto res = sess.Post();
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  auto form = res_json["form"];
  EXPECT_TRUE(form["k1"] == "v1");
  EXPECT_TRUE(form["k2"] == "v2");
}

TEST(MultipartTest, File) {
  // Create test file
  std::ofstream outfile("test.txt", std::ios::trunc);
  outfile << "test content";
  outfile.close();

  quest::Session sess;
  sess.SetUrl(quest::Url(hb_server + "/post"));
  sess.SetMultipart({{"test file", quest::File("test.txt")}});

  auto res = sess.Post();
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  auto files = res_json["files"];
  EXPECT_TRUE(files["test file"] == "test content");
}

TEST(MultipartTest, MixedForm) {
  // Create test file
  std::ofstream outfile("test.txt", std::ios::trunc);
  outfile << "test content";
  outfile.close();

  quest::Session sess;
  sess.SetUrl(quest::Url(hb_server + "/post"));
  sess.SetMultipart({{"test file", quest::File("test.txt")}, {"k1", "v1"}});

  auto res = sess.Post();
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  auto files = res_json["files"];
  auto form = res_json["form"];
  EXPECT_TRUE(form["k1"] == "v1");
  EXPECT_TRUE(files["test file"] == "test content");
}
