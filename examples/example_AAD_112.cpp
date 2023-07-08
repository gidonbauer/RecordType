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
  using Type = RT::RecordType<double>;
  Eigen::Vector2<Type> v{1.0, 2.0};
  f(v);

  save_to_dot<Type>(__FILE__);
}
