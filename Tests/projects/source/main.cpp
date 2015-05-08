#include "gtest/gtest.h"

namespace mytest {
int myadd(const int a, const int b)
{
  return a + b;
}
}
  
TEST(AddTest, PositiveNumber) {
  EXPECT_EQ(3,  mytest::myadd(1, 2));
  EXPECT_EQ(13, mytest::myadd(10, 3));
  EXPECT_EQ(20, mytest::myadd(9, 11));
}
 
TEST(AddTest, NegativeNumber) {
  EXPECT_EQ(-3,  mytest::myadd(-1, -2));
  EXPECT_EQ(-13, mytest::myadd(-10, -3));
  EXPECT_EQ(-20, mytest::myadd(-9, -11));
}
 
TEST(AddTest, ZeroNumber) {
  EXPECT_EQ(0, mytest::myadd(0, 0));
  EXPECT_EQ(1, mytest::myadd(1, 0));
  EXPECT_EQ(2, mytest::myadd(0, 2));
  EXPECT_EQ(-1, mytest::myadd(-1, 0));
  EXPECT_EQ(-2, mytest::myadd(-0, -2));
}
 
TEST(AddTest, PositiveAndNegativeNumber) {
  EXPECT_EQ(0, mytest::myadd(1, -1));
  EXPECT_EQ(1, mytest::myadd(-2, 3));
  EXPECT_EQ(2, mytest::myadd(-3, 5));
}