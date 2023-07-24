#include <gtest/gtest.h>

#include <numbers>

#include "RecordType.hpp"

TEST(test_RT_RecordType_Add, Int) {
  using PT    = int;
  using RType = RT::RecordType<PT>;
  {
    RType rt1(42);
    RType rt2(12);
    rt1 += rt2;
    EXPECT_EQ(rt1.value(), 54);
    EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);
  }

  {
    RType rt1(42);
    RType rt2(12);

    auto graph = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(graph);
    rt2.register_graph(graph);

    RType rt3 = rt1 + rt2;

    EXPECT_EQ(rt3.value(), 54);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }

  {
    RType rt1(42);
    RType rt2(12);
    auto graph = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(graph);
    rt2.register_graph(graph);

    RType rt3(rt1 + rt2);

    EXPECT_EQ(rt3.value(), 54);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }
}

TEST(test_RT_RecordType_Add, Double) {
  using PT    = int;
  using RType = RT::RecordType<PT>;
  {
    RType rt1(42.0);
    RType rt2(12.0);

    rt1 += rt2;
    EXPECT_DOUBLE_EQ(rt1.value(), 54.0);
    EXPECT_EQ(rt1.node_type(), RT::NodeType::VAR);
  }

  {
    RType rt1(42.0);
    RType rt2(12.0);

    auto graph = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(graph);
    rt2.register_graph(graph);

    RType rt3 = rt1 + rt2;

    EXPECT_DOUBLE_EQ(rt3.value(), 54.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }

  {
    RType rt1(42.0);
    RType rt2(12.0);

    auto graph = std::make_shared<RT::Graph<PT>>();
    rt1.register_graph(graph);
    rt2.register_graph(graph);

    RType rt3(rt1 + rt2);

    EXPECT_DOUBLE_EQ(rt3.value(), 54.0);
    EXPECT_NE(rt3.id(), rt1.id());
    EXPECT_NE(rt3.id(), rt2.id());
    EXPECT_EQ(rt3.node_type(), RT::NodeType::ADD);
  }
}
