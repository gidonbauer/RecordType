#include <iostream>

#include <Eigen/Dense>

#include "RecordType.hpp"

#include "save_to_dot.hpp"

template <typename MT>
constexpr auto f(const MT& x, const MT& y) noexcept -> MT {
  auto t = x + y;
  return t * x * y + t + x;
}

auto main() -> int {
  using PT    = Eigen::Matrix<double, 2, 2>;
  using RType = RT::RecordType<PT>;

  RType x = static_cast<PT>(PT::Random());
  RType y = static_cast<PT>(PT::Random());

  auto g = std::make_shared<RT::Graph<PT>>();
  x.register_graph(g);
  y.register_graph(g);

  RType res = f(x, y);

  save_to_dot(__FILE__, g.get());
}
