#include <algorithm>
#include <iostream>
#include <vector>

#include "RecordType.hpp"
#include "save_to_dot.hpp"

template <typename T>
auto dot_product(const std::vector<T>& a, const std::vector<T>& b) -> T {
  RT_ASSERT(a.size() == b.size(), "a and b must have the same size.");

  T res = 0;
  for (size_t i = 0; i < a.size(); ++i) {
    res += a[i] * b[i];
  }
  return res;
}

auto main() -> int {
  using namespace std::string_literals;
  using Type = RT::RecordType<float>;

  auto graph = std::make_shared<RT::Graph<float>>();

  constexpr size_t n = 5;
  std::vector<Type> vec1(n, 1.0);
  std::vector<Type> vec2(n, 2.0);

  std::for_each(std::begin(vec1), std::end(vec1), [&](auto& e) { e.register_graph(graph); });
  std::for_each(std::begin(vec2), std::end(vec2), [&](auto& e) { e.register_graph(graph); });

  [[maybe_unused]] Type res = dot_product(vec1, vec2);

  RT::GraphToDotOptions opt{
      .unique_literals      = true,
      .number_only_literals = true,
      .print_node_id        = true,
      .use_op_symbols       = true,
  };
  save_to_dot(__FILE__, graph.get(), opt);
}
