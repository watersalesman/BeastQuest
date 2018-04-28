#include <gtest/gtest.h>
#include <beastquest/session.hh>


TEST(SSLTest, VerifySSL) {
  quest::Session sess;
  sess.SetVerifySSL(true);
  quest::Url url("https://self-signed.badssl.com/");
  sess.SetUrl(url);

  ASSERT_ANY_THROW(sess.Get());

}

TEST(SSLTest, NoVerifySSL) {
  quest::Session sess;
  sess.SetVerifySSL(false);
  quest::Url url("https://self-signed.badssl.com/");
  sess.SetUrl(url);

  auto res = sess.Get();
  ASSERT_EQ(res.status_code, 200);

}

TEST(SSLTest, StandardRequest) {
  quest::Session sess;
  quest::Url url("https://httpbin.org/get");
  sess.SetUrl(url);
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
}

TEST(SSLTest, KeepAlive) {
  quest::Session sess;
  sess.SetKeepAlive(true);
  quest::Url url("https://httpbin.org/get");
  sess.SetUrl(url);
  sess.Get();
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 200); EXPECT_FALSE(res.content.empty());
}

TEST(SSLTest, KeepAliveRedirect) {
  quest::Session sess;
  sess.SetKeepAlive(true);
  quest::Url url("https://httpbin.org/redirect/2");
  sess.SetUrl(url);
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
}

TEST(SSLTest, KeepAliveTwoHosts) {
  quest::Session sess;
  sess.SetKeepAlive(true);
  quest::Url url("https://httpbin.org/get");
  sess.SetUrl(url);
  sess.Get();

  sess.SetUrl(quest::Url("https://google.com"));
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
}

TEST(SSLTest, NoKeepAlive) {
  quest::Session sess;
  sess.SetKeepAlive(false);
  quest::Url url("https://httpbin.org/get");
  sess.SetUrl(url);
  sess.Get();
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
}

TEST(SSLTest, NoKeepAliveRedirect) {
  quest::Session sess;
  sess.SetKeepAlive(false);
  quest::Url url("https://httpbin.org/redirect/2");
  sess.SetUrl(url);
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
}

TEST(SSLTest, NoKeepAliveTwoHosts) {
  quest::Session sess;
  sess.SetKeepAlive(false);
  quest::Url url("https://httpbin.org/get");
  sess.SetUrl(url);
  sess.Get();

  sess.SetUrl(quest::Url("https://github.com"));
  auto res = sess.Get();

  ASSERT_EQ(res.status_code, 200);
  EXPECT_FALSE(res.content.empty());
}