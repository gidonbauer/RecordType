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
  RT::register_variable(vec1, graph);
  RT::register_variable(vec2, graph);
  for (Eigen::Index i = 0; i < n; ++i) {
    RT::add_name(vec1(i), "vec1[" + std::to_string(i) + "]");
    RT::add_name(vec2(i), "vec2[" + std::to_string(i) + "]");
  }

  Type res = vec1.dot(vec2);
  RT::mark_output(res);
  RT::add_name(res, "res");

  save_to_dot(__FILE__, graph.get());
}
