#include <gtest/gtest.h>

#include <numbers>

#include "RecordType.hpp"

TEST(test_RT_RecordType, Constructor) {
  RT::RecordType rt1(42);
  EXPECT_EQ(rt1.value(), 42);
  EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);

  RT::RecordType rt2(-42.0f);
  EXPECT_FLOAT_EQ(rt2.value(), -42.0f);
  EXPECT_EQ(rt2.node_type(), RT::NodeType::VAR);
}

TEST(test_RT_RecordType, Assign) {
  {
    RT::RecordType rt(42);
    const auto old_value     = rt.value();
    const auto old_id        = rt.id();
    const auto old_node_type = rt.node_type();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-assign-overloaded"
    rt = rt;
#pragma GCC diagnostic pop

    EXPECT_EQ(old_value, rt.value());
    EXPECT_NE(old_id, rt.id());
    EXPECT_EQ(old_node_type, rt.node_type());
  }

  {
    RT::RecordType rt1(42);
    RT::RecordType rt2(12);
    RT::RecordType rt3(0);
    rt3 = rt1 + rt2;

    EXPECT_EQ(rt3.value(), 54);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::VAR);
  }

  {
    RT::RecordType rt1(2);
    RT::RecordType rt2(3);
    RT::RecordType rt3(0);
    rt3 = rt1 * rt2 + rt1;

    EXPECT_EQ(rt3.value(), 8);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::VAR);
  }
}

TEST(test_RT_RecordType, Add) {
  {
    RT::RecordType rt1(42);
    RT::RecordType rt2(12);
    rt1 += rt2;
    EXPECT_EQ(rt1.value(), 54);
    EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);
  }

  {
    RT::RecordType rt1(42.0);
    RT::RecordType rt2(12.0);
    rt1 += rt2;
    EXPECT_DOUBLE_EQ(rt1.value(), 54.0);
    EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);
  }

  {
    RT::RecordType rt1(42);
    RT::RecordType rt2(12);
    RT::RecordType rt3 = rt1 + rt2;

    EXPECT_EQ(rt3.value(), 54);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }

  {
    RT::RecordType rt1(42);
    RT::RecordType rt2(12);
    RT::RecordType rt3(rt1 + rt2);

    EXPECT_EQ(rt3.value(), 54);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }

  {
    RT::RecordType rt1(42.0);
    RT::RecordType rt2(12.0);
    RT::RecordType rt3 = rt1 + rt2;

    EXPECT_DOUBLE_EQ(rt3.value(), 54.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }

  {
    RT::RecordType rt1(42.0);
    RT::RecordType rt2(12.0);
    RT::RecordType rt3(rt1 + rt2);

    EXPECT_DOUBLE_EQ(rt3.value(), 54.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }
}

TEST(test_RT_RecordType, Mul) {
  {
    RT::RecordType rt1(2);
    RT::RecordType rt2(3);
    rt1 *= rt2;

    EXPECT_EQ(rt1.value(), 6);
    EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);
  }

  {
    RT::RecordType rt1(2.0);
    RT::RecordType rt2(3.0);
    rt1 *= rt2;
    EXPECT_DOUBLE_EQ(rt1.value(), 6.0);
    EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);
  }

  {
    RT::RecordType rt1(2);
    RT::RecordType rt2(3);
    RT::RecordType rt3 = rt1 * rt2;

    EXPECT_EQ(rt3.value(), 6);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::MUL);
  }

  {
    RT::RecordType rt1(2);
    RT::RecordType rt2(3);
    RT::RecordType rt3(rt1 * rt2);

    EXPECT_EQ(rt3.value(), 6);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::MUL);
  }

  {
    RT::RecordType rt1(2.0);
    RT::RecordType rt2(3.0);
    RT::RecordType rt3 = rt1 * rt2;

    EXPECT_DOUBLE_EQ(rt3.value(), 6.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::MUL);
  }

  {
    RT::RecordType rt1(2.0);
    RT::RecordType rt2(3.0);
    RT::RecordType rt3(rt1 * rt2);

    EXPECT_DOUBLE_EQ(rt3.value(), 6.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::MUL);
  }
}

TEST(test_RT_RecordType, Div) {
  // TODO: Add test for integer types when `invert` is fixed
  {
    RT::RecordType rt1(6.0);
    RT::RecordType rt2(3.0);
    rt1 /= rt2;

    EXPECT_DOUBLE_EQ(rt1.value(), 2.0);
    EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);
  }

  {
    RT::RecordType rt1(6.0);
    RT::RecordType rt2(3.0);
    RT::RecordType rt3 = rt1 / rt2;

    EXPECT_DOUBLE_EQ(rt3.value(), 2.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::MUL);
  }

  {
    RT::RecordType rt1(6.0);
    RT::RecordType rt2(3.0);
    RT::RecordType rt3(rt1 / rt2);

    EXPECT_DOUBLE_EQ(rt3.value(), 2.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::MUL);
  }
}

TEST(test_RT_RecordType, Sub) {
  // TODO: Add test for unsigned integer when negation is fixed
  {
    RT::RecordType rt1(6);
    RT::RecordType rt2(3);
    rt1 -= rt2;

    EXPECT_EQ(rt1.value(), 3);
    EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);
  }

  {
    RT::RecordType rt1(6.0);
    RT::RecordType rt2(3.0);
    rt1 -= rt2;

    EXPECT_DOUBLE_EQ(rt1.value(), 3.0);
    EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);
  }

  {
    RT::RecordType rt1(1);
    RT::RecordType rt2 = -rt1;

    EXPECT_EQ(rt2.value(), -1);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::NEG);
  }

  {
    RT::RecordType rt1(6);
    RT::RecordType rt2(3);
    RT::RecordType rt3 = rt1 - rt2;

    EXPECT_EQ(rt3.value(), 3);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }

  {
    RT::RecordType rt1(6);
    RT::RecordType rt2(3);
    RT::RecordType rt3(rt1 - rt2);

    EXPECT_EQ(rt3.value(), 3);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }

  {
    RT::RecordType rt1(1.0);
    RT::RecordType rt2 = -rt1;

    EXPECT_DOUBLE_EQ(rt2.value(), -1.0);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::NEG);
  }

  {
    RT::RecordType rt1(6.0);
    RT::RecordType rt2(3.0);
    RT::RecordType rt3 = rt1 - rt2;

    EXPECT_DOUBLE_EQ(rt3.value(), 3.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }

  {
    RT::RecordType rt1(6.0);
    RT::RecordType rt2(3.0);
    RT::RecordType rt3(rt1 - rt2);

    EXPECT_DOUBLE_EQ(rt3.value(), 3.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }
}

TEST(test_RT_RecordType, Sqrt) {
  {
    RT::RecordType rt1(4);
    RT::RecordType rt2 = sqrt(rt1);

    EXPECT_EQ(rt2.value(), 2);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SQRT);
  }

  {
    RT::RecordType rt1(2.0);
    RT::RecordType rt2 = sqrt(rt1);

    EXPECT_DOUBLE_EQ(rt2.value(), std::numbers::sqrt2_v<double>);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SQRT);
  }

  {
    RT::RecordType rt1(4);
    RT::RecordType rt2 = std::sqrt(rt1);

    EXPECT_EQ(rt2.value(), 2);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SQRT);
  }

  {
    RT::RecordType rt1(2.0);
    RT::RecordType rt2 = std::sqrt(rt1);

    EXPECT_DOUBLE_EQ(rt2.value(), std::numbers::sqrt2_v<double>);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SQRT);
  }
}

TEST(test_RT_RecordType, Compare) {
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
    RT::RecordType rt1(1);
    RT::RecordType rt2(1);
    EXPECT_FALSE(rt1 < rt2);
    EXPECT_FALSE(rt2 > rt1);
    EXPECT_TRUE(rt1 <= rt2);
    EXPECT_TRUE(rt2 >= rt1);
    EXPECT_FALSE(rt1 != rt2);
    EXPECT_TRUE(rt2 == rt1);
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
