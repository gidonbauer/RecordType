#include <iostream>

#include <Eigen/Dense>

#include "RecordType.hpp"

#include "save_to_dot.hpp"

template <typename T, int N>
constexpr auto gaussian_elimination(Eigen::Matrix<T, N, N> A, Eigen::Vector<T, N> b)
    -> Eigen::Vector<T, N> {
  if constexpr (N == Eigen::Dynamic) {
    RT_ASSERT(A.rows() == A.cols(),
              "A must be square, but dimension is (" << A.rows() << ", " << A.cols() << ")");
    RT_ASSERT(A.rows() == b.rows(),
              "Dimension of A and b must match, but dimensions are ("
                  << A.rows() << ", " << A.cols() << ") and " << b.rows());
  }
  const auto n = A.rows();

  for (Eigen::Index i = 0; i < n; ++i) {
    for (Eigen::Index j = i + 1; j < n; ++j) {
      const auto m = -(A(j, i) / A(i, i));
      b(j) += m * b(i);
      for (Eigen::Index k = i; k < n; ++k) {
        A(j, k) += m * A(i, k);
      }
    }
  }

  Eigen::Vector<T, N> x(n);

  for (Eigen::Index i = n - 1; i >= 0; --i) {
    auto bi = b(i);
    for (Eigen::Index j = n - 1; j > i; --j) {
      bi -= x(j) * A(i, j);
    }
    x(i) = bi / A(i, i);
  }

  return x;
}

auto main() -> int {
  using Passive_t = double;
  using Rec_t     = RT::RecordType<Passive_t>;

  auto graph = std::make_shared<RT::Graph<Passive_t>>();

  constexpr int N = 2;
  Eigen::Matrix<Rec_t, Eigen::Dynamic, Eigen::Dynamic> A(N, N);
  A << 1.0, 2.0, 3.0, 4.0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      RT::register_variable(A(i, j), graph);
      RT::add_name(A(i, j), std::format("A({0}, {1})", i, j));
    }
  }

  Eigen::Vector<Rec_t, Eigen::Dynamic> b(N);
  b << 5.0, 6.0;
  RT::register_variable(b, graph);
  for (int i = 0; i < N; ++i) {
    RT::add_name(b(i), std::format("b({0})", i));
  }

  Eigen::Vector<Rec_t, N> x = gaussian_elimination(A, b);
  RT::mark_output(x);
  for (int i = 0; i < N; ++i) {
    RT::add_name(x(i), std::format("x({0})", i));
  }

  std::cout << "A =\n";
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      std::cout << A(i, j) << ' ';
    }
    std::cout << '\n';
  }
  std::cout << '\n';

  std::cout << "b =\n";
  for (int i = 0; i < N; ++i) {
    std::cout << b(i) << '\n';
  }
  std::cout << '\n';

  std::cout << "x =\n";
  for (int i = 0; i < N; ++i) {
    std::cout << x(i) << '\n';
  }
  std::cout << '\n';

  save_to_dot(__FILE__, graph.get());
}
