#include <gtest/gtest.h>
#include <beastquest/session.hh>
#include <json.hpp>

using json = nlohmann::json;

TEST(RequestTest, GET) {
  quest::Session sess;
  quest::Url url("http://httpbin.org/get");
  sess.SetUrl(url);
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
  EXPECT_EQ(res.reason, "OK");
  EXPECT_FALSE(res.headers.empty());
}

TEST(RequestTest, POST) {
  quest::Session sess;
  quest::Url url("http://httpbin.org/post");
  sess.SetUrl(url);
  auto res = sess.Post();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
  EXPECT_EQ(res.reason, "OK");
  EXPECT_FALSE(res.headers.empty());
}

TEST(RequestTest, PUT) {
  quest::Session sess;
  quest::Url url("http://httpbin.org/put");
  sess.SetUrl(url);
  auto res = sess.Put();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
  EXPECT_EQ(res.reason, "OK");
  EXPECT_FALSE(res.headers.empty());
}

TEST(RequestTest, DELETE) {
  quest::Session sess;
  quest::Url url("http://httpbin.org/delete");
  sess.SetUrl(url);
  auto res = sess.Delete();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
  EXPECT_EQ(res.reason, "OK");
  EXPECT_FALSE(res.headers.empty());
}

TEST(RequestTest, PATCH) {
  quest::Session sess;
  quest::Url url("http://httpbin.org/patch");
  sess.SetUrl(url);
  auto res = sess.Patch();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
  EXPECT_EQ(res.reason, "OK");
  EXPECT_FALSE(res.headers.empty());
}

TEST(RequestTest, FollowRedirects) {
  quest::Session sess;
  quest::Url url("http://httpbin.org/redirect/3");
  sess.SetUrl(url);
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 200);
}

TEST(RequestTest, NoRedirects) {
  quest::Session sess;
  sess.SetMaxRedirects(0);
  quest::Url url("http://httpbin.org/redirect/1");
  sess.SetUrl(url);
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 302);
}

TEST(RequestTest, MaxRedirects) {
  quest::Session sess;
  sess.SetMaxRedirects(3);
  quest::Url url("http://httpbin.org/redirect/4");
  sess.SetUrl(url);
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 302);
  ASSERT_EQ(res.headers["Location"], "/get");
}

TEST(KeepAliveTest, NoKeepAlive) {
  quest::Session sess;
  sess.SetKeepAlive(false);
  quest::Url url("http://httpbin.org/get");
  sess.SetUrl(url);
  sess.Get();
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
}

TEST(KeepAliveTest, NoKeepAliveRedirect) {
  quest::Session sess;
  sess.SetKeepAlive(false);
  quest::Url url("http://httpbin.org/redirect/2");
  sess.SetUrl(url);
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
}

TEST(OptionsTest, CustomHeaders) {
  quest::Session sess;
  sess.SetUrl(quest::Url("http://httpbin.org/headers"));
  sess.SetHeaders({{"Header1", "one"}, {"Header2", "two"}});
  sess.SetUserAgent("My User Agent");

  auto res = sess.Get();
  ASSERT_EQ(res.status_code, 200);

  auto res_json = json::parse(res.content);
  auto headers = res_json["headers"];
  EXPECT_TRUE(headers["User-Agent"] == "My User Agent");
  EXPECT_TRUE(headers["Header1"] == "one");
  EXPECT_TRUE(headers["Header2"] == "two");
}
