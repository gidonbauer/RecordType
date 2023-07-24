#include <gtest/gtest.h>

#include "Graph.hpp"
#include "RecordType.hpp"

TEST(test_RT_Graph, SimpleAddition) {
  using Rec_t = RT::RecordType<int>;
  const Rec_t rt0(42);
  const Rec_t rt1(-42);

  auto graph = std::make_shared<RT::Graph<int>>();
  rt0.register_graph(graph);
  rt1.register_graph(graph);

  const Rec_t rt2 = rt0 + rt1;

  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  ASSERT_EQ(ops.size(), 3ul);
  EXPECT_EQ(ops[0], rt0.node_type());
  EXPECT_EQ(ops[1], rt1.node_type());
  EXPECT_EQ(ops[2], rt2.node_type());

  ASSERT_EQ(vals.size(), 3ul);
  EXPECT_EQ(vals[0], rt0.value());
  EXPECT_EQ(vals[1], rt1.value());
  EXPECT_EQ(vals[2], rt2.value());

  ASSERT_EQ(deps.size(), 3ul + 1ul + 2ul);
  EXPECT_EQ(deps[0], rt0.id());
  EXPECT_EQ(deps[1], rt1.id());

  EXPECT_EQ(deps[2], rt0.id());
  EXPECT_EQ(deps[3], rt1.id());
  EXPECT_EQ(deps[4], -2);
  EXPECT_EQ(deps[5], rt2.id());
}

TEST(test_RT_Graph, Copy) {
  using Rec_t = RT::RecordType<int>;
  const Rec_t rt0(42);
  auto graph = std::make_shared<RT::Graph<int>>();
  rt0.register_graph(graph);

  const Rec_t rt1 = rt0;

  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  ASSERT_EQ(ops.size(), 2ul);
  EXPECT_EQ(ops[0], rt0.node_type());
  EXPECT_EQ(ops[1], rt1.node_type());

  ASSERT_EQ(vals.size(), 2ul);
  EXPECT_EQ(vals[0], rt0.value());
  EXPECT_EQ(vals[1], rt1.value());

  ASSERT_EQ(deps.size(), 2ul + 1ul + 1ul);
  EXPECT_EQ(deps[0], rt0.id());

  EXPECT_EQ(deps[1], rt0.id());
  EXPECT_EQ(deps[2], -1);
  EXPECT_EQ(deps[3], rt1.id());
}
