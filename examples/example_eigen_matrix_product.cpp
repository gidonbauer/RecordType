#include <iostream>

#include <Eigen/Dense>

#include "RecordType.hpp"
#include "save_to_dot.hpp"

auto main() -> int {
  using namespace std::string_literals;
  using Type = RT::RecordType<double>;

  constexpr Eigen::Index n = 2;
  Eigen::MatrixX<Type> lhs(n, n);
  lhs << 1.0, 2.0, 3.0, 4.0;
  Eigen::MatrixX<Type> rhs(n, n);
  rhs << 5.0, 6.0, 7.0, 8.0;

  auto graph = std::make_shared<RT::Graph<double>>();
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      lhs(i, j).register_graph(graph);
      rhs(i, j).register_graph(graph);
    }
  }

  Eigen::MatrixX<Type> res = lhs * rhs;

  RT::GraphToDotOptions opt{
      .print_node_id  = true,
      .use_op_symbols = true,
  };
  save_to_dot(__FILE__, graph.get(), opt);
}
