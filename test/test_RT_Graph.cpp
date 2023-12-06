#include <gtest/gtest.h>

#include "Graph.hpp"
#include "RecordType.hpp"

TEST(test_RT_Graph, DifferentGraphs) {
  using PT    = int;
  using RType = RT::RecordType<PT>;

  auto graph1 = std::make_shared<RT::Graph<PT>>();
  const RType rt1(42);
  rt1.register_graph(graph1);

  auto graph2 = std::make_shared<RT::Graph<PT>>();
  const RType rt2(-42);
  rt2.register_graph(graph2);

  const RType rt3 = rt1 + rt2;

  EXPECT_NE(rt1.id(), RT::UNREGISTERED) << "rt1 was not properly registered";
  EXPECT_NE(rt2.id(), RT::UNREGISTERED) << "rt2 was not properly registered";
  EXPECT_EQ(rt3.id(), RT::UNREGISTERED) << "rt3 should not have been registered";

  EXPECT_NE(rt1.graph(), nullptr) << "rt1 should have a graph";
  EXPECT_NE(rt2.graph(), nullptr) << "rt2 should have a graph";
  EXPECT_NE(rt1.graph(), rt2.graph()) << "rt1 and rt2 should be in different graphs";

  EXPECT_EQ(rt3.graph(), nullptr) << "rt3 should not have a graph";

  EXPECT_EQ(rt3.value(), 0);

  // ASSERT_EQ(graph1->dependencies().size(), 1ul);
  // EXPECT_EQ(graph1->dependencies()[0], rt1.id());

  // ASSERT_EQ(graph1->values().size(), 1ul);
  // EXPECT_EQ(graph1->values()[0], 42);

  // ASSERT_EQ(graph1->operations().size(), 1ul);
  // EXPECT_EQ(graph1->operations()[0], RT::NodeType::VAR);

  // ASSERT_EQ(graph2->dependencies().size(), 1ul);
  // EXPECT_EQ(graph2->dependencies()[0], rt2.id());

  // ASSERT_EQ(graph2->values().size(), 1ul);
  // EXPECT_EQ(graph2->values()[0], -42);

  // ASSERT_EQ(graph2->operations().size(), 1ul);
  // EXPECT_EQ(graph2->operations()[0], RT::NodeType::VAR);
}

TEST(test_RT_Graph, SimpleAddition) {
  using Rec_t = RT::RecordType<int>;

  const Rec_t rt0(42);
  const Rec_t rt1(-42);

  auto graph = std::make_shared<RT::Graph<int>>();
  rt0.register_graph(graph);
  rt1.register_graph(graph);

  const Rec_t rt2 = rt0 + rt1;

  // const auto& deps = graph->dependencies();
  // const auto& ops  = graph->operations();
  // const auto& vals = graph->values();

  // ASSERT_EQ(ops.size(), 3ul);
  // EXPECT_EQ(ops[0], rt0.node_type());
  // EXPECT_EQ(ops[1], rt1.node_type());
  // EXPECT_EQ(ops[2], rt2.node_type());

  // ASSERT_EQ(vals.size(), 3ul);
  // EXPECT_EQ(vals[0], rt0.value());
  // EXPECT_EQ(vals[1], rt1.value());
  // EXPECT_EQ(vals[2], rt2.value());

  // ASSERT_EQ(deps.size(), 3ul + 1ul + 2ul);
  // EXPECT_EQ(deps[0], rt0.id());
  // EXPECT_EQ(deps[1], rt1.id());

  // EXPECT_EQ(deps[2], rt0.id());
  // EXPECT_EQ(deps[3], rt1.id());
  // EXPECT_EQ(deps[4], -2);
  // EXPECT_EQ(deps[5], rt2.id());
}

TEST(test_RT_Graph, Copy) {
  using Rec_t = RT::RecordType<int>;
  const Rec_t rt0(42);
  auto graph = std::make_shared<RT::Graph<int>>();
  rt0.register_graph(graph);

  const Rec_t rt1 = rt0;

  // const auto& deps = graph->dependencies();
  // const auto& ops  = graph->operations();
  // const auto& vals = graph->values();

  // ASSERT_EQ(ops.size(), 2ul);
  // EXPECT_EQ(ops[0], rt0.node_type());
  // EXPECT_EQ(ops[1], rt1.node_type());

  // ASSERT_EQ(vals.size(), 2ul);
  // EXPECT_EQ(vals[0], rt0.value());
  // EXPECT_EQ(vals[1], rt1.value());

  // ASSERT_EQ(deps.size(), 2ul + 1ul + 1ul);
  // EXPECT_EQ(deps[0], rt0.id());

  // EXPECT_EQ(deps[1], rt0.id());
  // EXPECT_EQ(deps[2], -1);
  // EXPECT_EQ(deps[3], rt1.id());
}
