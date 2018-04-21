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