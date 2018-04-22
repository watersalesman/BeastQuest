#include <gtest/gtest.h>
#include <beastquest/util.hh>

TEST(UtilTest, CaseInsensitiveCompare) {
  using namespace quest::detail;
  CaseInsensitiveCompare comp;
  ASSERT_FALSE(comp("Hello","hello"));
  ASSERT_FALSE(comp("hello","Hello"));
  ASSERT_TRUE(comp("apple","banana"));
}

TEST(UtilTest, UrlEncode) {
  using namespace quest::detail;
  std::string unencoded = "!*'();:@&=+$,/?%#[]";
  std::string encoded =
      "%21%2A%27%28%29%3B%3A%40%26%3D%2B%24%2C%2F%3F%25%23%5B%5D";

  // Neither string should be "less than" the other
  CaseInsensitiveCompare comp;
  ASSERT_FALSE(comp(UrlEncode(unencoded), encoded));
  ASSERT_FALSE(comp(encoded, UrlEncode(unencoded)));
}