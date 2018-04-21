#include <gtest/gtest.h>
#include <beastquest/util.hh>

TEST(UtilTest, CaseInsensitiveCompare) {
  quest::CaseInsensitiveCompare comp;
  ASSERT_FALSE(comp("Hello","hello"));
  ASSERT_FALSE(comp("hello","Hello"));
  ASSERT_TRUE(comp("apple","banana"));
}

TEST(UtilTest, UrlEncode) {
  std::string unencoded = "!*'();:@&=+$,/?%#[]";
  std::string encoded =
      "%21%2A%27%28%29%3B%3A%40%26%3D%2B%24%2C%2F%3F%25%23%5B%5D";

  // Neither string should be "less than" the other
  quest::CaseInsensitiveCompare comp;
  ASSERT_FALSE(comp(quest::UrlEncode(unencoded), encoded));
  ASSERT_FALSE(comp(encoded, quest::UrlEncode(unencoded)));
}