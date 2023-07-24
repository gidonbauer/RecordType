#include <iostream>

#include "RecordType.hpp"
#include "save_to_dot.hpp"

auto main() -> int {
  using Type = RT::RecordType<double>;

  auto g = std::make_shared<RT::Graph<double>>();

  Type rt1 = 5;
  Type rt2 = 2;
  rt1.register_graph(g);
  rt2.register_graph(g);

  [[maybe_unused]] Type rt3 = rt1 - rt2;

  save_to_dot(__FILE__, g.get());
}
