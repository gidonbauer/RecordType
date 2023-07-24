#include <gtest/gtest.h>

#include <numbers>

#include "RecordType.hpp"

TEST(test_RT_RecordType_Sqrt, Int) {
  using PT    = int;
  using RType = RT::RecordType<PT>;
  {
    RType rt1(4);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = sqrt(rt1);

    EXPECT_EQ(rt2.value(), 2);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SQRT);
  }

  {
    RType rt1(4);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = std::sqrt(rt1);

    EXPECT_EQ(rt2.value(), 2);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SQRT);
  }
}

TEST(test_RT_RecordType_Sqrt, Double) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  {
    RType rt1(2.0);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = sqrt(rt1);

    EXPECT_DOUBLE_EQ(rt2.value(), std::numbers::sqrt2_v<double>);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SQRT);
  }

  {
    RType rt1(2.0);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = std::sqrt(rt1);

    EXPECT_DOUBLE_EQ(rt2.value(), std::numbers::sqrt2_v<double>);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::SQRT);
  }
}
