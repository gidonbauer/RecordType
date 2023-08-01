#include <iostream>

#include <Eigen/Dense>

#include "RecordType.hpp"
#include "ToPython.hpp"

template <typename T>
void f(Eigen::Vector2<T>& v) {
  T u;
  u    = v(0) * v(0) * v(1) * v(1);
  v(0) = (std::sin(u) + std::cos(u)) / (v(0) - v(1));
  v(1) = std::sqrt(v(1) * u);
}

auto main() -> int {
  using PassiveType = double;
  using RType       = RT::RecordType<PassiveType>;

  Eigen::Vector2<RType> v{1.0, 2.0};
  std::cout << "f(" << v(0).value() << ", " << v(1).value() << ") = ";

  auto graph = std::make_shared<RT::Graph<PassiveType>>();
  RT::register_variable(v, graph);

  f(v);

  std::cout << "[" << v(0).value() << ", " << v(1).value() << "]\n";

  try {
    const std::string filename{"test.py"};
    RT::to_python(graph.get(), filename);
    std::cout << "Wrote Graph code to `" << filename << "`.\n";
  } catch (const std::exception& e) {
    std::cerr << "[ERROR] " << e.what() << '\n';
    std::exit(1);
  }
}
