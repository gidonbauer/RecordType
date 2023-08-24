#include <algorithm>
#include <iostream>
#include <vector>

#include "RecordType.hpp"
#include "save_to_dot.hpp"

template <typename T>
using Matrix = std::vector<std::vector<T>>;

template <typename T>
auto mat_mult(const Matrix<T>& lhs, const Matrix<T>& rhs) -> Matrix<T> {
  RT_ASSERT(lhs.size() >= 1ul, "");
  RT_ASSERT(rhs.size() >= 1ul, "");
  RT_ASSERT(rhs[0].size() >= 1ul, "");
  RT_ASSERT(lhs[0].size() == rhs.size(), "");

  const size_t m = lhs.size();
  const size_t n = rhs[0].size();
  const size_t k = lhs[0].size();

  Matrix<T> res(m, std::vector<T>(n, static_cast<T>(0)));

  for (size_t i = 0ul; i < m; ++i) {
    for (size_t j = 0ul; j < n; ++j) {
      for (size_t o = 0ul; o < k; ++o) {
        res[i][j] += lhs[i][o] * rhs[o][j];
      }
    }
  }

  return res;
}

auto main() -> int {
  using Type = RT::RecordType<double>;

  Matrix<Type> lhs{
      {1.0, 2.0},
      {3.0, 4.0},
  };
  Matrix<Type> rhs{
      {5.0, 6.0},
      {7.0, 8.0},
  };

  auto graph = std::make_shared<RT::Graph<double>>();

  std::for_each(std::cbegin(lhs), std::cend(lhs), [&](const auto& vec) {
    std::for_each(std::begin(vec), std::end(vec), [&](auto& e) { e.register_graph(graph); });
  });
  std::for_each(std::cbegin(rhs), std::cend(rhs), [&](const auto& vec) {
    std::for_each(std::begin(vec), std::end(vec), [&](auto& e) { e.register_graph(graph); });
  });

  [[maybe_unused]] auto res = mat_mult(lhs, rhs);

  RT::GraphToDotOptions opt{
      .print_node_id  = true,
      .use_op_symbols = true,
  };
  save_to_dot(__FILE__, graph.get(), opt);
}
