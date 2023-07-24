#include <gtest/gtest.h>

#include <numbers>

#include "RecordType.hpp"

TEST(test_RT_RecordType_Sin, Int) {
  using PT    = int;
  using RType = RT::RecordType<PT>;
  {
    RType rt1(0);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = sin(rt1);

    EXPECT_EQ(rt2.value(), 0);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SIN);
  }

  {
    RType rt1(0);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = std::sin(rt1);

    EXPECT_EQ(rt2.value(), 0);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SIN);
  }
}

TEST(test_RT_RecordType_Sin, Double) {
  using PT    = double;
  using RType = RT::RecordType<PT>;
  {
    RType rt1(std::numbers::pi / 2.0);
    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = sin(rt1);

    EXPECT_DOUBLE_EQ(rt2.value(), 1.0);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SIN);
  }

  {
    RType rt1(std::numbers::pi / 2.0);
    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = std::sin(rt1);

    EXPECT_DOUBLE_EQ(rt2.value(), 1.0);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SIN);
  }
}
