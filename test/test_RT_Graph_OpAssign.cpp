#include <gtest/gtest.h>

#include "RecordType.hpp"

TEST(test_RT_Graph, PlusAssignAllRegistered) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType y = 1.0;
  RType x = 2.0;

  auto graph = std::make_shared<RT::Graph<PT>>();
  RT::register_variable(x, graph);
  RT::register_variable(y, graph);

  const auto orig_y_id = y.id();

  y += x;

  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  // - deps -------------------------------------
  ASSERT_EQ(deps.size(), 9ul);
  auto d = deps.cbegin();
  EXPECT_EQ(*d++, x.id());
  EXPECT_EQ(*d++, orig_y_id);

  EXPECT_EQ(*d++, orig_y_id);
  EXPECT_EQ(*d++, x.id());
  EXPECT_EQ(*d++, -2);
  EXPECT_NE(*d++, RT::UNREGISTERED);

  EXPECT_NE(*d++, RT::UNREGISTERED);
  EXPECT_EQ(*d++, -1);
  EXPECT_EQ(*d++, y.id());
  // - deps -------------------------------------

  // - ops --------------------------------------
  ASSERT_EQ(ops.size(), 4ul);
  auto o = ops.cbegin();
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  EXPECT_EQ(*o++, RT::NodeType::ADD);
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  // - ops --------------------------------------

  // - vals -------------------------------------
  ASSERT_EQ(vals.size(), 4ul);
  auto v = vals.cbegin();
  EXPECT_DOUBLE_EQ(*v++, 2.0);
  EXPECT_DOUBLE_EQ(*v++, 1.0);
  EXPECT_DOUBLE_EQ(*v++, 3.0);
  EXPECT_DOUBLE_EQ(*v++, 3.0);
  // - vals -------------------------------------
}

TEST(test_RT_Graph, PlusAssignRhsUnregistered) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType y = 1.0;
  RType x = 2.0;

  auto graph = std::make_shared<RT::Graph<PT>>();
  RT::register_variable(y, graph);

  const auto orig_y_id = y.id();

  y += x;

  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  // - deps -------------------------------------
  ASSERT_EQ(deps.size(), 9ul);
  auto d = deps.cbegin();
  EXPECT_EQ(*d++, orig_y_id);

  EXPECT_EQ(*d++, x.id());

  EXPECT_EQ(*d++, orig_y_id);
  EXPECT_EQ(*d++, x.id());
  EXPECT_EQ(*d++, -2);
  EXPECT_NE(*d++, RT::UNREGISTERED);

  EXPECT_NE(*d++, RT::UNREGISTERED);
  EXPECT_EQ(*d++, -1);
  EXPECT_EQ(*d++, y.id());
  // - deps -------------------------------------

  // - ops --------------------------------------
  ASSERT_EQ(ops.size(), 4ul);
  auto o = ops.cbegin();
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  EXPECT_EQ(*o++, RT::NodeType::ADD);
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  // - ops --------------------------------------

  // - vals -------------------------------------
  ASSERT_EQ(vals.size(), 4ul);
  auto v = vals.cbegin();
  EXPECT_DOUBLE_EQ(*v++, 1.0);
  EXPECT_DOUBLE_EQ(*v++, 2.0);
  EXPECT_DOUBLE_EQ(*v++, 3.0);
  EXPECT_DOUBLE_EQ(*v++, 3.0);
  // - vals -------------------------------------
}

TEST(test_RT_Graph, PlusAssignLhsUnregistered) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType y = 1.0;
  RType x = 2.0;

  auto graph = std::make_shared<RT::Graph<PT>>();
  RT::register_variable(x, graph);

  y += x;

  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  // - deps -------------------------------------
  ASSERT_EQ(deps.size(), 9ul);
  auto d = deps.cbegin();
  EXPECT_EQ(*d++, x.id());

  EXPECT_NE(*d++, RT::UNREGISTERED);

  EXPECT_NE(*d++, RT::UNREGISTERED);
  EXPECT_EQ(*d++, x.id());
  EXPECT_EQ(*d++, -2);
  EXPECT_NE(*d++, RT::UNREGISTERED);

  EXPECT_NE(*d++, RT::UNREGISTERED);
  EXPECT_EQ(*d++, -1);
  EXPECT_EQ(*d++, y.id());
  // - deps -------------------------------------

  // - ops --------------------------------------
  ASSERT_EQ(ops.size(), 4ul);
  auto o = ops.cbegin();
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  EXPECT_EQ(*o++, RT::NodeType::ADD);
  EXPECT_EQ(*o++, RT::NodeType::VAR);
  // - ops --------------------------------------

  // - vals -------------------------------------
  ASSERT_EQ(vals.size(), 4ul);
  auto v = vals.cbegin();
  EXPECT_DOUBLE_EQ(*v++, 2.0);
  EXPECT_DOUBLE_EQ(*v++, 1.0);
  EXPECT_DOUBLE_EQ(*v++, 3.0);
  EXPECT_DOUBLE_EQ(*v++, 3.0);
  // - vals -------------------------------------
}
