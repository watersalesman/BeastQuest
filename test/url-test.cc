#include <gtest/gtest.h>
#include <beastquest/url.hh>

TEST(UrlTest, InvalidUrl) {
  EXPECT_ANY_THROW(quest::Url("this isn't a url"));
  // Proper format but invalid schema
  EXPECT_ANY_THROW(quest::Url("schema://url.stuff:4444/stuff"));
}

TEST(UrlTest, ValidUrl) {
  EXPECT_NO_THROW(quest::Url("http://example.com"));
  EXPECT_NO_THROW(quest::Url("https://example.com"));
  EXPECT_NO_THROW(quest::Url("https://example.com:8080"));
  EXPECT_NO_THROW(quest::Url("https://example.com:8080/stuff?hello=hello"));
}

TEST(UrlTest, ParseUrl) {
  quest::Url url;

  url = quest::Url("https://example.com");
  EXPECT_EQ(url.use_ssl, true);
  EXPECT_EQ(url.host, "example.com");
  EXPECT_EQ(url.port, "443");
  EXPECT_EQ(url.path, "/");

  url = quest::Url("http://example.com/stuff?k1=v1");
  EXPECT_EQ(url.use_ssl, false);
  EXPECT_EQ(url.host, "example.com");
  EXPECT_EQ(url.port, "80");
  EXPECT_EQ(url.path, "/stuff?k1=v1");

  url = quest::Url("https://example.com:4444");
  EXPECT_EQ(url.use_ssl, true);
  EXPECT_EQ(url.port, "4444");
}

TEST(UrlTest, CannotShareLoopbackConnection) {
  quest::Url loop_url1("http://localhost");
  quest::Url loop_url2("http://127.0.0.1");
  quest::Url other_url("http://example.com");

  EXPECT_FALSE(loop_url1.CanShareConnectionWith(loop_url2));
  EXPECT_FALSE(loop_url1.CanShareConnectionWith(other_url));
  EXPECT_FALSE(loop_url2.CanShareConnectionWith(loop_url1));
  EXPECT_FALSE(loop_url2.CanShareConnectionWith(other_url));
  EXPECT_FALSE(other_url.CanShareConnectionWith(loop_url1));
  EXPECT_FALSE(other_url.CanShareConnectionWith(loop_url2));
}

TEST(UrlTest, CanShareNormalConnection) {
  quest::Url url1("http://example.com:80/page1");
  quest::Url url2("http://example.com:80/page2");

  EXPECT_TRUE(url1.CanShareConnectionWith(url2));
}

TEST(UrlTest, CannotShareNormalConnection) {
  quest::Url url1("http://example.com:80");
  quest::Url url2("http://example.com:8080");
  quest::Url url3("https://example.com");
  quest::Url url4("http://other-example.com");

  EXPECT_FALSE(url1.CanShareConnectionWith(url2));
  EXPECT_FALSE(url1.CanShareConnectionWith(url3));
  EXPECT_FALSE(url1.CanShareConnectionWith(url4));
}