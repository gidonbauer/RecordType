#include <gtest/gtest.h>

#include "RecordType.hpp"

TEST(test_RT_Graph, IntermediateRegister) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType x = 1.0;
  RType y = 2.0;
  RType z;

  auto graph = std::make_shared<RT::Graph<PT>>();
  RT::register_variable(x, graph);
  RT::register_variable(y, graph);
  RT::register_variable(z, graph);
  const auto orig_z_id = z.id();

  z = x + y;

  RType u;
  RT::register_variable(u, graph);
  const auto orig_u_id = u.id();

  u = x * y * z;

  // const auto& deps = graph->dependencies();
  // const auto& ops  = graph->operations();
  // const auto& vals = graph->values();

  // - deps -------------------------------------
  // ASSERT_EQ(deps.size(), 22ul);
  // auto d = deps.cbegin();
  // EXPECT_EQ(*d++, x.id());
  // EXPECT_EQ(*d++, y.id());
  // EXPECT_EQ(*d++, orig_z_id);

  // EXPECT_EQ(*d++, x.id());
  // EXPECT_EQ(*d++, y.id());
  // EXPECT_EQ(*d++, -2);
  // EXPECT_NE(*d++, RT::UNREGISTERED);

  // EXPECT_NE(*d++, RT::UNREGISTERED);
  // EXPECT_EQ(*d++, -1);
  // EXPECT_EQ(*d++, z.id());

  // EXPECT_EQ(*d++, orig_u_id);

  // EXPECT_EQ(*d++, x.id());
  // EXPECT_EQ(*d++, y.id());
  // EXPECT_EQ(*d++, -2);
  // EXPECT_NE(*d++, RT::UNREGISTERED);

  // EXPECT_NE(*d++, RT::UNREGISTERED);
  // EXPECT_EQ(*d++, z.id());
  // EXPECT_EQ(*d++, -2);
  // EXPECT_NE(*d++, RT::UNREGISTERED);

  // EXPECT_NE(*d++, RT::UNREGISTERED);
  // EXPECT_EQ(*d++, -1);
  // EXPECT_EQ(*d++, u.id());
  // - deps -------------------------------------

  // - ops --------------------------------------
  // ASSERT_EQ(ops.size(), 9ul);
  // auto o = ops.cbegin();
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::ADD);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::MUL);
  // EXPECT_EQ(*o++, RT::NodeType::MUL);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // - ops --------------------------------------

  // - vals -------------------------------------
  // ASSERT_EQ(vals.size(), 9ul);
  // auto v = vals.cbegin();
  // EXPECT_DOUBLE_EQ(*v++, 1.0);
  // EXPECT_DOUBLE_EQ(*v++, 2.0);
  // EXPECT_DOUBLE_EQ(*v++, 0.0);
  // EXPECT_DOUBLE_EQ(*v++, 3.0);
  // EXPECT_DOUBLE_EQ(*v++, 3.0);
  // EXPECT_DOUBLE_EQ(*v++, 0.0);
  // EXPECT_DOUBLE_EQ(*v++, 2.0);
  // EXPECT_DOUBLE_EQ(*v++, 6.0);
  // EXPECT_DOUBLE_EQ(*v++, 6.0);
  // - vals -------------------------------------
}

TEST(test_RT_Graph, NoIntermediateRegister) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType x = 1.0;
  RType y = 2.0;
  RType z;

  auto graph = std::make_shared<RT::Graph<PT>>();
  RT::register_variable(x, graph);
  RT::register_variable(y, graph);
  RT::register_variable(z, graph);
  const auto orig_z_id = z.id();

  z = x + y;

  RType u;
  u = x * y * z;

  // const auto& deps = graph->dependencies();
  // const auto& ops  = graph->operations();
  // const auto& vals = graph->values();

  // - deps -------------------------------------
  // ASSERT_EQ(deps.size(), 21ul);
  // auto d = deps.cbegin();
  // EXPECT_EQ(*d++, x.id());
  // EXPECT_EQ(*d++, y.id());
  // EXPECT_EQ(*d++, orig_z_id);

  // EXPECT_EQ(*d++, x.id());
  // EXPECT_EQ(*d++, y.id());
  // EXPECT_EQ(*d++, -2);
  // EXPECT_NE(*d++, RT::UNREGISTERED);

  // EXPECT_NE(*d++, RT::UNREGISTERED);
  // EXPECT_EQ(*d++, -1);
  // EXPECT_EQ(*d++, z.id());

  // EXPECT_EQ(*d++, x.id());
  // EXPECT_EQ(*d++, y.id());
  // EXPECT_EQ(*d++, -2);
  // EXPECT_NE(*d++, RT::UNREGISTERED);

  // EXPECT_NE(*d++, RT::UNREGISTERED);
  // EXPECT_EQ(*d++, z.id());
  // EXPECT_EQ(*d++, -2);
  // EXPECT_NE(*d++, RT::UNREGISTERED);

  // EXPECT_NE(*d++, RT::UNREGISTERED);
  // EXPECT_EQ(*d++, -1);
  // EXPECT_EQ(*d++, u.id());
  // - deps -------------------------------------

  // - ops --------------------------------------
  // ASSERT_EQ(ops.size(), 8ul);
  // auto o = ops.cbegin();
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::ADD);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::MUL);
  // EXPECT_EQ(*o++, RT::NodeType::MUL);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // - ops --------------------------------------

  // - vals -------------------------------------
  // ASSERT_EQ(vals.size(), 8ul);
  // auto v = vals.cbegin();
  // EXPECT_DOUBLE_EQ(*v++, 1.0);
  // EXPECT_DOUBLE_EQ(*v++, 2.0);
  // EXPECT_DOUBLE_EQ(*v++, 0.0);
  // EXPECT_DOUBLE_EQ(*v++, 3.0);
  // EXPECT_DOUBLE_EQ(*v++, 3.0);
  // EXPECT_DOUBLE_EQ(*v++, 2.0);
  // EXPECT_DOUBLE_EQ(*v++, 6.0);
  // EXPECT_DOUBLE_EQ(*v++, 6.0);
  // - vals -------------------------------------
}

TEST(test_RT_Graph, NoIntermediateRegister2) {
  using PT    = double;
  using RType = RT::RecordType<PT>;

  RType x = 1.0;
  RType y = 2.0;
  RType z;

  auto graph = std::make_shared<RT::Graph<PT>>();
  RT::register_variable(x, graph);
  RT::register_variable(y, graph);
  RT::register_variable(z, graph);
  const auto orig_z_id = z.id();

  z = x + y;

  RType u = x * y * z;

  EXPECT_EQ(u.node_type(), RT::NodeType::MUL);

  // const auto& deps = graph->dependencies();
  // const auto& ops  = graph->operations();
  // const auto& vals = graph->values();

  // - deps -------------------------------------
  // ASSERT_EQ(deps.size(), 18ul);
  // auto d = deps.cbegin();
  // EXPECT_EQ(*d++, x.id());
  // EXPECT_EQ(*d++, y.id());
  // EXPECT_EQ(*d++, orig_z_id);

  // EXPECT_EQ(*d++, x.id());
  // EXPECT_EQ(*d++, y.id());
  // EXPECT_EQ(*d++, -2);
  // EXPECT_NE(*d++, RT::UNREGISTERED);

  // EXPECT_NE(*d++, RT::UNREGISTERED);
  // EXPECT_EQ(*d++, -1);
  // EXPECT_EQ(*d++, z.id());

  // EXPECT_EQ(*d++, x.id());
  // EXPECT_EQ(*d++, y.id());
  // EXPECT_EQ(*d++, -2);
  // EXPECT_NE(*d++, RT::UNREGISTERED);

  // EXPECT_NE(*d++, RT::UNREGISTERED);
  // EXPECT_EQ(*d++, z.id());
  // EXPECT_EQ(*d++, -2);
  // EXPECT_EQ(*d++, u.id());
  // - deps -------------------------------------

  // - ops --------------------------------------
  // ASSERT_EQ(ops.size(), 7ul);
  // auto o = ops.cbegin();
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::ADD);
  // EXPECT_EQ(*o++, RT::NodeType::VAR);
  // EXPECT_EQ(*o++, RT::NodeType::MUL);
  // EXPECT_EQ(*o++, RT::NodeType::MUL);
  // - ops --------------------------------------

  // - vals -------------------------------------
  // ASSERT_EQ(vals.size(), 7l);
  // auto v = vals.cbegin();
  // EXPECT_DOUBLE_EQ(*v++, 1.0);
  // EXPECT_DOUBLE_EQ(*v++, 2.0);
  // EXPECT_DOUBLE_EQ(*v++, 0.0);
  // EXPECT_DOUBLE_EQ(*v++, 3.0);
  // EXPECT_DOUBLE_EQ(*v++, 3.0);
  // EXPECT_DOUBLE_EQ(*v++, 2.0);
  // EXPECT_DOUBLE_EQ(*v++, 6.0);
  // - vals -------------------------------------
}
