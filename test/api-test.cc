#include <gtest/gtest.h>
#include <beastquest/beastquest.hh>
#include <fstream>
#include <json.hpp>

#ifdef BeastQuest_LOCAL_HTTPBIN
std::string hb_server = "http://127.0.0.1:8000";
#endif
#ifndef BeastQuest_LOCAL_HTTPBIN
std::string hb_server = "http://httpbin.org";
#endif

using json = nlohmann::json;

TEST(SyncAPI, GET) {
  auto res = quest::Get(quest::Url(hb_server + "/get"));
  ASSERT_EQ(res.status_code, 200);
}

TEST(SyncAPI, POST) {
  auto res = quest::Post(quest::Url(hb_server + "/post"));
  ASSERT_EQ(res.status_code, 200);
}

TEST(SyncAPI, DELETE) {
  auto res = quest::Delete(quest::Url(hb_server + "/delete"));
  ASSERT_EQ(res.status_code, 200);
}

TEST(SyncAPI, PUT) {
  auto res = quest::Put(quest::Url(hb_server + "/put"));
  ASSERT_EQ(res.status_code, 200);
}

TEST(SyncAPI, PATCH) {
  auto res = quest::Patch(quest::Url(hb_server + "/patch"));
  ASSERT_EQ(res.status_code, 200);
}

TEST(AsyncAPI, GET) {
  auto res_future = quest::AsyncGet(quest::Url(hb_server + "/get"));
  ASSERT_EQ(res_future.get().status_code, 200);
}

TEST(AsyncAPI, POST) {
  auto res_future = quest::AsyncPost(quest::Url(hb_server + "/post"));
  ASSERT_EQ(res_future.get().status_code, 200);
}

TEST(AsyncAPI, DELETE) {
  auto res_future = quest::AsyncDelete(quest::Url(hb_server + "/delete"));
  ASSERT_EQ(res_future.get().status_code, 200);
}

TEST(AsyncAPI, PUT) {
  auto res_future = quest::AsyncPut(quest::Url(hb_server + "/put"));
  ASSERT_EQ(res_future.get().status_code, 200);
}

TEST(AsyncAPI, PATCH) {
  auto res_future = quest::AsyncPatch(quest::Url(hb_server + "/patch"));
  ASSERT_EQ(res_future.get().status_code, 200);
}

TEST(CallbackAPI, GET) {
  auto res_status_f =
      quest::CallbackGet([](quest::Response res) { return res.status_code; },
                         quest::Url(hb_server + "/get"));
  ASSERT_EQ(res_status_f.get(), 200);
}

TEST(CallbackAPI, POST) {
  auto res_status_f =
      quest::CallbackPost([](quest::Response res) { return res.status_code; },
                          quest::Url(hb_server + "/post"));
  ASSERT_EQ(res_status_f.get(), 200);
}

TEST(CallbackAPI, DELETE) {
  auto res_status_f =
      quest::CallbackDelete([](quest::Response res) { return res.status_code; },
                            quest::Url(hb_server + "/delete"));
  ASSERT_EQ(res_status_f.get(), 200);
}

TEST(CallbackAPI, PUT) {
  auto res_status_f =
      quest::CallbackPut([](quest::Response res) { return res.status_code; },
                         quest::Url(hb_server + "/put"));
  ASSERT_EQ(res_status_f.get(), 200);
}

TEST(CallbackAPI, PATCH) {
  auto res_status_f =
      quest::CallbackPatch([](quest::Response res) { return res.status_code; },
                           quest::Url(hb_server + "/patch"));
  ASSERT_EQ(res_status_f.get(), 200);
}

TEST(OptionsTest, Headers) {
  auto res =
      quest::Get(quest::Url(hb_server + "/headers"),
                 quest::Headers({{"Header1", "one"}, {"Header2", "two"}}));
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  auto headers = res_json["headers"];
  EXPECT_TRUE(headers["Header1"] == "one");
  EXPECT_TRUE(headers["Header2"] == "two");
}

TEST(OptionsTest, Body) {
  auto res =
      quest::Post(quest::Url(hb_server + "/post"), quest::Body("Hello there"));
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  EXPECT_TRUE(res_json["data"] == "Hello there");
}

TEST(OptionsTest, Parameters) {
  auto res = quest::Get(quest::Url(hb_server + "/get"),
                        quest::Parameters({{"k1", "v1"}, {"k2", "v2"}}));
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  auto params = res_json["args"];
  EXPECT_TRUE(params["k1"] == "v1");
  EXPECT_TRUE(params["k2"] == "v2");
}

TEST(OptionsTest, SimpleForm) {
  auto res = quest::Post(quest::Url(hb_server + "/post"),
                         quest::SimpleForm({{"k1", "v1"}, {"k2", "v2"}}));
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  auto form = res_json["form"];
  EXPECT_TRUE(form["k1"] == "v1");
  EXPECT_TRUE(form["k2"] == "v2");
}

TEST(OptionsTest, Multipart) {
  // Create test file
  std::ofstream outfile("test.txt", std::ios::trunc);
  outfile << "test content";
  outfile.close();

  auto res = quest::Post(
      quest::Url(hb_server + "/post"),
      quest::Multipart({{"test file", quest::File("test.txt")}, {"k1", "v1"}}));
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  auto files = res_json["files"];
  auto form = res_json["form"];
  EXPECT_TRUE(form["k1"] == "v1");
  EXPECT_TRUE(files["test file"] == "test content");
}

TEST(OptionsTest, BasicAuth) {
  auto res = quest::Get(quest::Url(hb_server + "/basic-auth/user/pass"),
                        quest::BasicAuth("user", "pass"));
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  EXPECT_TRUE(res_json["authenticated"] == true);
}

TEST(OptionsTest, MaxRedirects) {
  auto res =
      quest::Get(quest::Url(hb_server + "/redirect/4"), quest::MaxRedirects(3));

  ASSERT_EQ(res.status_code, 302);
  ASSERT_EQ(res.headers["Location"], "/get");
}

TEST(OptionsTest, UserAgent) {
  auto res = quest::Get(quest::Url(hb_server + "/headers"),
                        quest::UserAgent("My User Agent"));

  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  auto headers = res_json["headers"];
  EXPECT_TRUE(headers["User-Agent"] == "My User Agent");
}
