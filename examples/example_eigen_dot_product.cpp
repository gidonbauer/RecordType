#include <iostream>

#include <Eigen/Dense>

#include "RecordType.hpp"
#include "save_to_dot.hpp"

auto main() -> int {
  using Type = RT::RecordType<double>;

  constexpr Eigen::Index n  = 5;
  Eigen::VectorX<Type> vec1 = Eigen::VectorX<Type>::Constant(n, 1.0);
  Eigen::VectorX<Type> vec2 = Eigen::VectorX<Type>::Constant(n, 2.0);

  [[maybe_unused]] Type res = vec1.dot(vec2);

  RT::GraphToDotOptions opt{
      .unique_literals      = true,
      .number_only_literals = true,
      .print_node_id        = true,
      .use_op_symbols       = true,
  };
  save_to_dot<Type>(__FILE__, opt);
}
