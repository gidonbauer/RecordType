#include <iostream>

#include <Eigen/Dense>

#include "RecordType.hpp"

#include "save_to_dot.hpp"

template <typename T>
void f(Eigen::Vector2<T>& v) {
  T u;
  u    = v(0) * v(0) * v(1) * v(1);
  v(0) = std::sin(u);
  v(1) = v(1) * u;
}

auto main() -> int {
  using PassiveType = double;
  using RType       = RT::RecordType<PassiveType>;

  Eigen::Vector2<RType> v{1.0, 2.0};

  auto graph = std::make_shared<RT::Graph<PassiveType>>();
  v(0).register_graph(graph);
  v(1).register_graph(graph);

  f(v);

  RT::mark_output(v);

  graph->dump_data(std::cout);
  save_to_dot(__FILE__, graph.get());
}
