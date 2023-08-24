#include <gtest/gtest.h>

#include "Macros.hpp"

TEST(TestRTAssert, ShouldPass) {
  int i    = 3;
  double d = 4.2;

  // clang-format off
  EXPECT_NO_FATAL_FAILURE(
      RT_ASSERT(i * d > 12.0, "3 * 4.2 is greater than 12");
  );

  EXPECT_NO_FATAL_FAILURE(
      RT_ASSERT(i * d < 13.0, "3 * 4.2 is less than 13");
  );

  EXPECT_NO_FATAL_FAILURE(
      RT_ASSERT((std::is_same_v<decltype(i * d), double>), "Type of expression should be double");
  );
  // clang-format on
}

TEST(TestRTAssert, ShouldFail) {
  int i    = 3;
  double d = 4.2;

  // clang-format off
  EXPECT_EXIT(
      RT_ASSERT(i * d < 12.0, "3 * 4.2 is greater than 12");,
      ::testing::KilledBySignal(SIGABRT),
      ".+[ASSERT].+:30.+`i \\* d < 12.0` failed: 3 \\* 4.2 is greater than 12\n"
  );

  EXPECT_EXIT(
      RT_ASSERT(i * d > 13.0, "3 * 4.2 is less than 13");,
      ::testing::KilledBySignal(SIGABRT),
      ".+[ASSERT].+:36.+`i \\* d > 13.0` failed: 3 \\* 4.2 is less than 13\n"
  );

  EXPECT_EXIT(
      RT_ASSERT((std::is_same_v<decltype(i * d), int>), "Type of expression should be double");,
      ::testing::KilledBySignal(SIGABRT),
      ".+[ASSERT].+:42.+`\\(std::is_same_v<decltype\\(i \\* d\\), int>\\)` failed: Type of expression should be double\n"
  );
  // clang-format on
}