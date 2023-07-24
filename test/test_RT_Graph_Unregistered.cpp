#include <gtest/gtest.h>

#include "Graph.hpp"
#include "RecordType.hpp"

TEST(test_RT_Graph_Unregistered, AdditionWithUnregistered) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType rt1 = 1.0;
  RType rt2 = 2.0;

  auto graph = std::make_shared<RT::Graph<PT>>();
  rt1.register_graph(graph);

  RType rt3 = rt1 + rt2;

  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  EXPECT_NE(rt1.id(), RT::UNREGISTERED) << "rt1 was not properly registered in the graph";
  EXPECT_NE(rt2.id(), RT::UNREGISTERED) << "rt2 was not properly registered in the graph";
  EXPECT_NE(rt3.id(), RT::UNREGISTERED) << "rt3 was not properly registered in the graph";

  ASSERT_EQ(deps.size(), 6ul);
  EXPECT_EQ(deps[0], rt1.id());
  EXPECT_EQ(deps[1], rt2.id());

  EXPECT_EQ(deps[2], rt1.id());
  EXPECT_EQ(deps[3], rt2.id());
  EXPECT_EQ(deps[4], -2);
  EXPECT_EQ(deps[5], rt3.id());

  ASSERT_EQ(ops.size(), 3ul);
  EXPECT_EQ(ops[0], RT::NodeType::VAR);
  EXPECT_EQ(ops[1], RT::NodeType::VAR);
  EXPECT_EQ(ops[2], RT::NodeType::ADD);

  ASSERT_EQ(vals.size(), 3ul);
  EXPECT_DOUBLE_EQ(vals[0], 1.0);
  EXPECT_DOUBLE_EQ(vals[1], 2.0);
  EXPECT_DOUBLE_EQ(vals[2], 3.0);
}

TEST(test_RT_Graph_Unregistered, MultiplicationWithUnregistered) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType rt1 = 5.0;
  RType rt2 = 3.0;

  auto graph = std::make_shared<RT::Graph<PT>>();
  rt1.register_graph(graph);

  RType rt3 = rt1 * rt2;

  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  EXPECT_NE(rt1.id(), RT::UNREGISTERED) << "rt1 was not properly registered in the graph";
  EXPECT_NE(rt2.id(), RT::UNREGISTERED) << "rt2 was not properly registered in the graph";
  EXPECT_NE(rt3.id(), RT::UNREGISTERED) << "rt3 was not properly registered in the graph";

  ASSERT_EQ(deps.size(), 6ul);
  EXPECT_EQ(deps[0], rt1.id());
  EXPECT_EQ(deps[1], rt2.id());

  EXPECT_EQ(deps[2], rt1.id());
  EXPECT_EQ(deps[3], rt2.id());
  EXPECT_EQ(deps[4], -2);
  EXPECT_EQ(deps[5], rt3.id());

  ASSERT_EQ(ops.size(), 3ul);
  EXPECT_EQ(ops[0], RT::NodeType::VAR);
  EXPECT_EQ(ops[1], RT::NodeType::VAR);
  EXPECT_EQ(ops[2], RT::NodeType::MUL);

  ASSERT_EQ(vals.size(), 3ul);
  EXPECT_DOUBLE_EQ(vals[0], 5.0);
  EXPECT_DOUBLE_EQ(vals[1], 3.0);
  EXPECT_DOUBLE_EQ(vals[2], 15.0);
}

TEST(test_RT_Graph_Unregistered, SubtractionWithUnregisteredRhs) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType rt1 = 3.0;
  RType rt2 = 5.0;

  auto graph = std::make_shared<RT::Graph<PT>>();
  rt1.register_graph(graph);

  RType rt3 = rt1 - rt2;

  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  EXPECT_NE(rt1.id(), RT::UNREGISTERED) << "rt1 was not properly registered in the graph";
  EXPECT_EQ(rt2.id(), RT::UNREGISTERED) << "rt2 should not be registered in the graph";
  EXPECT_NE(rt3.id(), RT::UNREGISTERED) << "rt3 was not properly registered in the graph";

  ASSERT_EQ(deps.size(), 6ul);
  EXPECT_EQ(deps[0], rt1.id());
  EXPECT_NE(deps[1], RT::UNREGISTERED);

  EXPECT_EQ(deps[2], rt1.id());
  EXPECT_NE(deps[3], RT::UNREGISTERED);
  EXPECT_EQ(deps[4], -2);
  EXPECT_EQ(deps[5], rt3.id());

  ASSERT_EQ(ops.size(), 3ul);
  EXPECT_EQ(ops[0], RT::NodeType::VAR);
  EXPECT_EQ(ops[1], RT::NodeType::NEG);
  EXPECT_EQ(ops[2], RT::NodeType::ADD);

  ASSERT_EQ(vals.size(), 3ul);
  EXPECT_DOUBLE_EQ(vals[0], 3.0);
  EXPECT_DOUBLE_EQ(vals[1], -5.0);
  EXPECT_DOUBLE_EQ(vals[2], -2.0);
}

TEST(test_RT_Graph_Unregistered, SubtractionWithUnregisteredLhs) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType rt1 = 3.0;
  RType rt2 = 5.0;

  auto graph = std::make_shared<RT::Graph<PT>>();
  rt2.register_graph(graph);

  RType rt3 = rt1 - rt2;

  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  EXPECT_NE(rt1.id(), RT::UNREGISTERED) << "rt1 was not properly registered in the graph";
  EXPECT_NE(rt2.id(), RT::UNREGISTERED) << "rt2 was not properly registered in the graph";
  EXPECT_NE(rt3.id(), RT::UNREGISTERED) << "rt3 was not properly registered in the graph";

  // - deps -------------------------------------
  ASSERT_EQ(deps.size(), 9ul);
  auto d = deps.cbegin();

  EXPECT_EQ(*d++, rt2.id());

  EXPECT_EQ(*d++, rt2.id());
  EXPECT_EQ(*d++, -1);
  auto tmp_id = *d++;
  EXPECT_NE(tmp_id, RT::UNREGISTERED);

  EXPECT_EQ(*d++, rt1.id());

  EXPECT_EQ(*d++, rt1.id());
  EXPECT_EQ(*d++, tmp_id);
  EXPECT_EQ(*d++, -2);
  EXPECT_EQ(*d++, rt3.id());
  // - deps -------------------------------------

  // - ops --------------------------------------
  ASSERT_EQ(ops.size(), 4ul);
  auto o = ops.cbegin();
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  EXPECT_EQ(*o++, RT::NodeType::NEG);
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  EXPECT_EQ(*o++, RT::NodeType::ADD);
  // - ops --------------------------------------

  // - vals -------------------------------------
  ASSERT_EQ(vals.size(), 4ul);
  auto v = vals.cbegin();
  EXPECT_DOUBLE_EQ(*v++, 5.0);
  EXPECT_DOUBLE_EQ(*v++, -5.0);
  EXPECT_DOUBLE_EQ(*v++, 3.0);
  EXPECT_DOUBLE_EQ(*v++, -2.0);
  // - vals -------------------------------------
}

TEST(test_RT_Graph_Unregistered, DivisionWithUnregisteredRhs) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType rt1 = 3.0;
  RType rt2 = 5.0;

  auto graph = std::make_shared<RT::Graph<PT>>();
  rt1.register_graph(graph);

  RType rt3 = rt1 / rt2;

  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  EXPECT_NE(rt1.id(), RT::UNREGISTERED) << "rt1 was not properly registered in the graph";
  EXPECT_EQ(rt2.id(), RT::UNREGISTERED) << "rt2 should not be registered in the graph";
  EXPECT_NE(rt3.id(), RT::UNREGISTERED) << "rt3 was not properly registered in the graph";

  // - deps -------------------------------------
  ASSERT_EQ(deps.size(), 6ul);
  auto d = deps.cbegin();
  EXPECT_EQ(*d++, rt1.id());

  auto tmp_id = *d++;
  EXPECT_NE(tmp_id, RT::UNREGISTERED);

  EXPECT_EQ(*d++, rt1.id());
  EXPECT_EQ(*d++, tmp_id);
  EXPECT_EQ(*d++, -2);
  EXPECT_EQ(*d++, rt3.id());
  // - deps -------------------------------------

  // - ops --------------------------------------
  ASSERT_EQ(ops.size(), 3ul);
  auto o = ops.cbegin();
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  EXPECT_EQ(*o++, RT::NodeType::INV);
  EXPECT_EQ(*o++, RT::NodeType::MUL);
  // - ops --------------------------------------

  // - vals -------------------------------------
  ASSERT_EQ(vals.size(), 3ul);
  auto v = vals.cbegin();
  EXPECT_DOUBLE_EQ(*v++, 3.0);
  EXPECT_DOUBLE_EQ(*v++, 1.0 / 5.0);
  EXPECT_DOUBLE_EQ(*v++, 3.0 / 5.0);
  // - vals -------------------------------------
}

TEST(test_RT_Graph_Unregistered, DivisionWithUnregisteredLhs) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType rt1 = 3.0;
  RType rt2 = 5.0;

  auto graph = std::make_shared<RT::Graph<PT>>();
  rt2.register_graph(graph);

  RType rt3 = rt1 / rt2;

  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  EXPECT_NE(rt1.id(), RT::UNREGISTERED) << "rt1 was not properly registered in the graph";
  EXPECT_NE(rt2.id(), RT::UNREGISTERED) << "rt2 was not properly registered in the graph";
  EXPECT_NE(rt3.id(), RT::UNREGISTERED) << "rt3 was not properly registered in the graph";

  // - deps -------------------------------------
  ASSERT_EQ(deps.size(), 9ul);
  auto d = deps.cbegin();
  EXPECT_EQ(*d++, rt2.id());

  EXPECT_EQ(*d++, rt2.id());
  EXPECT_EQ(*d++, -1);
  auto tmp_id = *d++;
  EXPECT_NE(tmp_id, RT::UNREGISTERED);

  EXPECT_EQ(*d++, rt1.id());

  EXPECT_EQ(*d++, rt1.id());
  EXPECT_EQ(*d++, tmp_id);
  EXPECT_EQ(*d++, -2);
  EXPECT_EQ(*d++, rt3.id());
  // - deps -------------------------------------

  // - ops --------------------------------------
  ASSERT_EQ(ops.size(), 4ul);
  auto o = ops.cbegin();
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  EXPECT_EQ(*o++, RT::NodeType::INV);
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  EXPECT_EQ(*o++, RT::NodeType::MUL);
  // - ops --------------------------------------

  // - vals -------------------------------------
  ASSERT_EQ(vals.size(), 4ul);
  auto v = vals.cbegin();
  EXPECT_DOUBLE_EQ(*v++, 5.0);
  EXPECT_DOUBLE_EQ(*v++, 1.0 / 5.0);
  EXPECT_DOUBLE_EQ(*v++, 3.0);
  EXPECT_DOUBLE_EQ(*v++, 3.0 / 5.0);
  // - vals -------------------------------------
}
