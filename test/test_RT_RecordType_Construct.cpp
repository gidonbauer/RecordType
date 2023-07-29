#include <gtest/gtest.h>

#include <numbers>

#include "RecordType.hpp"

TEST(test_RT_RecordType_Construct, Constructor) {
  RT::RecordType rt1(42);
  EXPECT_EQ(rt1.value(), 42);
  EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);

  RT::RecordType rt2(-42.0f);
  EXPECT_FLOAT_EQ(rt2.value(), -42.0f);
  EXPECT_EQ(rt2.node_type(), RT::NodeType::VAR);
}

TEST(test_RT_RecordType_Construct, Assign) {
  {
    RT::RecordType rt(42);

    auto graph = std::make_shared<RT::Graph<int>>();
    rt.register_graph(graph);

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
    RT::RecordType rt(42);

    auto graph = std::make_shared<RT::Graph<int>>();
    rt.register_graph(graph);

    const auto old_value     = rt.value();
    const auto old_id        = rt.id();
    const auto old_node_type = rt.node_type();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"
    rt = std::move(rt);
#pragma GCC diagnostic pop

    EXPECT_EQ(old_value, rt.value());
    EXPECT_NE(old_id, rt.id());
    EXPECT_EQ(old_node_type, rt.node_type());
  }

  {
    RT::RecordType rt1(42);
    RT::RecordType rt2(12);
    RT::RecordType rt3(0);

    auto graph = std::make_shared<RT::Graph<int>>();
    rt1.register_graph(graph);
    rt2.register_graph(graph);
    rt3.register_graph(graph);

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

    auto graph = std::make_shared<RT::Graph<int>>();
    rt1.register_graph(graph);
    rt2.register_graph(graph);
    rt3.register_graph(graph);

    rt3 = rt1 * rt2 + rt1;

    EXPECT_EQ(rt3.value(), 8);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::VAR);
  }
}

TEST(test_RT_RecordType_Construct, AssignPassive) {
  using PT    = double;
  using RType = RT::RecordType<PT>;
  RType rt    = 42.0;
  rt          = 21.0;
  EXPECT_DOUBLE_EQ(rt.value(), 21.0);
}
