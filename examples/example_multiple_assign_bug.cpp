#include <iostream>
#include <vector>

#include "RecordType.hpp"
#include "save_to_dot.hpp"

auto main() -> int {
  using Type = RT::RecordType<double>;
  Type rt1   = 4.2;

  auto graph = std::make_shared<RT::Graph<double>>();
  rt1.register_graph(graph);

  Type rt2(rt1);

  RT_DEBUG_PRINT(rt1.value());
  RT_DEBUG_PRINT(rt1.id());
  RT_DEBUG_PRINT(rt1.node_type());
  std::cout << '\n';
  RT_DEBUG_PRINT(rt2.value());
  RT_DEBUG_PRINT(rt2.id());
  RT_DEBUG_PRINT(rt2.node_type());

  RT::GraphToDotOptions opt{
      .print_node_id = true,
  };
  save_to_dot(__FILE__, graph.get(), opt);
}
