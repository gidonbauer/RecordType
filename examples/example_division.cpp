#include <iostream>

#include "RecordType.hpp"
#include "save_to_dot.hpp"

auto main() -> int {
  using Type = RT::RecordType<double>;

  Type rt1 = 4.0;
  Type rt2 = 2.0;

  [[maybe_unused]] Type rt3 = rt1 / rt2;

  save_to_dot<Type>(__FILE__);
}
