#include <iostream>

#include <Eigen/Dense>

#include "EigenInterop.hpp"
#include "RecordType.hpp"
#include "ToPython.hpp"

#include "save_to_dot.hpp"

template <typename MT, typename VT>
[[nodiscard]] constexpr auto gaussian_elimination(MT A, VT b) -> VT {
  RT_ASSERT(A.rows() == A.cols(),
            "A must be square, but dimension is (" << A.rows() << ", " << A.cols() << ")");
  RT_ASSERT(A.rows() == b.rows(),
            "Dimension of A and b must match, but dimensions are (" << A.rows() << ", " << A.cols()
                                                                    << ") and " << b.rows());
  const auto n = A.rows();
  VT x(n);

  for (Eigen::Index i = 0; i < n; ++i) {
    for (Eigen::Index j = i + 1; j < n; ++j) {
      const auto m = -(A(j, i) / A(i, i));
      b(j) += m * b(i);
      for (Eigen::Index k = i; k < n; ++k) {
        A(j, k) += m * A(i, k);
      }
    }
  }

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

  constexpr int N         = 10;
  Eigen::MatrixX<Rec_t> A = Eigen::MatrixX<Passive_t>::Random(N, N);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      RT::register_variable(A(i, j), graph);
      RT::add_name(A(i, j), std::format("A({0}, {1})", i, j));
    }
  }

  Eigen::VectorX<Rec_t> b = Eigen::VectorX<Rec_t>::Random(N);
  RT::register_variable(b, graph);
  for (int i = 0; i < N; ++i) {
    RT::add_name(b(i), std::format("b({0})", i));
  }

  Eigen::VectorX<Rec_t> x = gaussian_elimination(A, b);
  RT::mark_output(x);
  for (int i = 0; i < N; ++i) {
    RT::add_name(x(i), std::format("x({0})", i));
  }

  if constexpr (N <= 10) {
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
  }

  try {
    const std::string filename{"python/gaussian_elimination.py"};
    RT::to_python(graph.get(), filename);
    std::cout << "Wrote Graph code to `" << filename << "`.\n";
  } catch (const std::exception& e) {
    std::cerr << "[ERROR] " << e.what() << '\n';
    std::exit(1);
  }
  save_to_dot(__FILE__, graph.get());
}
