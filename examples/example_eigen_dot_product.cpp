#include <iostream>

#include <Eigen/Dense>

#include "RecordType.hpp"
#include "save_to_dot.hpp"

auto main() -> int {
  using Type = RT::RecordType<double>;

  constexpr Eigen::Index n  = 5;
  Eigen::VectorX<Type> vec1 = Eigen::VectorX<Type>::Constant(n, 1.0);
  Eigen::VectorX<Type> vec2 = Eigen::VectorX<Type>::Constant(n, 2.0);

  auto graph = std::make_shared<RT::Graph<double>>();
  std::for_each(std::begin(vec1), std::end(vec1), [&](auto& e) { e.register_graph(graph); });
  std::for_each(std::begin(vec2), std::end(vec2), [&](auto& e) { e.register_graph(graph); });

  [[maybe_unused]] Type res = vec1.dot(vec2);

  RT::GraphToDotOptions opt{
      .unique_literals      = true,
      .number_only_literals = true,
      .print_node_id        = true,
      .use_op_symbols       = true,
  };
  save_to_dot(__FILE__, graph.get(), opt);
}
