#include <gtest/gtest.h>

#include <numbers>

#include "RecordType.hpp"

TEST(test_RT_RecordType_Cos, Int) {
  using PT    = int;
  using RType = RT::RecordType<PT>;
  {
    RType rt1(0);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = cos(rt1);

    EXPECT_EQ(rt2.value(), 1);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::COS);
  }

  {
    RType rt1(0);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = std::cos(rt1);

    EXPECT_EQ(rt2.value(), 1);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::COS);
  }

  {
    RType rt1(0);
    RType rt2 = std::cos(rt1);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);
    rt2.register_graph(g);

    EXPECT_EQ(rt2.value(), 1);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::COS);
  }
}

TEST(test_RT_RecordType_Cos, Double) {
  using PT    = double;
  using RType = RT::RecordType<PT>;
  {
    RType rt1(std::numbers::pi / 2.0);
    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = cos(rt1);

    EXPECT_NEAR(rt2.value(), 0.0, 1e-16);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::COS);
  }

  {
    RType rt1(std::numbers::pi / 2.0);
    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = std::cos(rt1);

    EXPECT_NEAR(rt2.value(), 0.0, 1e-16);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::COS);
  }
}
