#include <gtest/gtest.h>

#include <numbers>

#include "RecordType.hpp"

TEST(test_RT_RecordType_Compare, Int) {
  {
    RT::RecordType rt1(1);
    RT::RecordType rt2(2);
    EXPECT_TRUE(rt1 < rt2);
    EXPECT_TRUE(rt2 > rt1);
    EXPECT_TRUE(rt1 <= rt2);
    EXPECT_TRUE(rt2 >= rt1);
    EXPECT_TRUE(rt1 != rt2);
    EXPECT_FALSE(rt2 == rt1);
  }

  {
    RT::RecordType rt1(1);
    RT::RecordType rt2(1);
    EXPECT_FALSE(rt1 < rt2);
    EXPECT_FALSE(rt2 > rt1);
    EXPECT_TRUE(rt1 <= rt2);
    EXPECT_TRUE(rt2 >= rt1);
    EXPECT_FALSE(rt1 != rt2);
    EXPECT_TRUE(rt2 == rt1);
  }
}

TEST(test_RT_RecordType_Compare, Double) {
  {
    RT::RecordType rt1(1.0);
    RT::RecordType rt2(2.0);
    EXPECT_TRUE(rt1 < rt2);
    EXPECT_TRUE(rt2 > rt1);
    EXPECT_TRUE(rt1 <= rt2);
    EXPECT_TRUE(rt2 >= rt1);
    EXPECT_TRUE(rt1 != rt2);
    EXPECT_FALSE(rt2 == rt1);
  }

  {
    RT::RecordType rt1(1.0);
    RT::RecordType rt2(1.0);
    EXPECT_FALSE(rt1 < rt2);
    EXPECT_FALSE(rt2 > rt1);
    EXPECT_TRUE(rt1 <= rt2);
    EXPECT_TRUE(rt2 >= rt1);
    EXPECT_FALSE(rt1 != rt2);
    EXPECT_TRUE(rt2 == rt1);
  }
}
