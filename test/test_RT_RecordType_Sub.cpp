#include <gtest/gtest.h>

#include <numbers>

#include "RecordType.hpp"

TEST(test_RT_RecordType_Sub, Int) {
  // TODO: Add test for unsigned integer when negation is fixed
  using PT    = int;
  using RType = RT::RecordType<PT>;
  {
    RType rt1(6);
    RType rt2(3);
    rt1 -= rt2;

    EXPECT_EQ(rt1.value(), 3);
    EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);
  }

  {
    RType rt1(1);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = -rt1;

    EXPECT_EQ(rt2.value(), -1);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::NEG);
  }

  {
    RType rt1(6);
    RType rt2(3);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);
    rt2.register_graph(g);

    RType rt3 = rt1 - rt2;

    EXPECT_EQ(rt3.value(), 3);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::SUB);
  }

  {
    RType rt1(6);
    RType rt2(3);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);
    rt2.register_graph(g);

    RType rt3(rt1 - rt2);

    EXPECT_EQ(rt3.value(), 3);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::SUB);
  }
}

TEST(test_RT_RecordType_Sub, Double) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  {
    RType rt1(6.0);
    RType rt2(3.0);
    rt1 -= rt2;

    EXPECT_DOUBLE_EQ(rt1.value(), 3.0);
    EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);
  }

  {
    RType rt1(1.0);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);

    RType rt2 = -rt1;

    EXPECT_DOUBLE_EQ(rt2.value(), -1.0);
    EXPECT_NE(rt1.id(), rt2.id());
    EXPECT_EQ(rt2.node_type(), RT::NodeType::NEG);
  }

  {
    RType rt1(6.0);
    RType rt2(3.0);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);
    rt2.register_graph(g);

    RType rt3 = rt1 - rt2;

    EXPECT_DOUBLE_EQ(rt3.value(), 3.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::SUB);
  }

  {
    RType rt1(6.0);
    RType rt2(3.0);

    auto g = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(g);
    rt2.register_graph(g);

    RType rt3(rt1 - rt2);

    EXPECT_DOUBLE_EQ(rt3.value(), 3.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::SUB);
  }
}
