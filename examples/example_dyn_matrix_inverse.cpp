#include <iostream>
#include <limits>
#include <random>

#include <Eigen/Dense>

#include "RecordType.hpp"
#include "TypeTraits.hpp"
#include "save_to_dot.hpp"

// - Check if a matrix is an identity matrix -------------------------------------------------------
template <typename Float>
requires(!RT::is_record_type_v<Float>)
auto is_identity(const Eigen::MatrixX<Float>& mat) noexcept -> bool {
  const Float tol = std::sqrt(std::numeric_limits<Float>::epsilon());

  for (Eigen::Index row = 0; row < mat.rows(); ++row) {
    for (Eigen::Index col = 0; col < mat.cols(); ++col) {
      if (row == col) {
        if (std::abs(mat(row, col) - static_cast<Float>(1.0)) > tol) {
          return false;
        }
      } else {
        if (std::abs(mat(row, col)) > tol) {
          return false;
        }
      }
    }
  }

  return true;
}

// - Generate random symmetric positive definite matrix --------------------------------------------
template <typename Float>
auto generate_random_spd_matrix(Eigen::Index size) -> Eigen::MatrixX<Float> {
  using DecayFloat = RT::decay_record_type_t<Float>;

  static const auto seed = std::random_device{}();
  static std::mt19937 generator(seed);
  std::uniform_real_distribution<DecayFloat> distribution(0.0, 1.0);

  const Eigen::MatrixX<Float> temp =
      Eigen::MatrixX<Float>::NullaryExpr(size, size, [&] { return distribution(generator); });

  return 0.5 * (temp + temp.transpose()) +
         static_cast<Float>(size) * Eigen::MatrixX<Float>::Identity(size, size);
}

// - Dynamic matrix inverse ------------------------------------------------------------------------
template <typename Float>
auto dynamic_matrix_inverse(const Eigen::MatrixX<Float>& A) noexcept -> Eigen::MatrixX<Float> {
  RT_ASSERT(A.rows() == A.cols(), "Matrix must be square.");

  const Eigen::Index n = A.rows();
  const auto all       = Eigen::seq(0, n - 1);

  Eigen::MatrixX<Float> M     = Eigen::MatrixX<Float>::Identity(n, n);
  Eigen::MatrixX<Float> M_inv = Eigen::MatrixX<Float>::Identity(n, n);

  Eigen::VectorX<Float> b(n, 1);
  Eigen::VectorX<Float> B_inv_col(n, 1);
  Eigen::RowVectorX<Float> active_row(1, n);
  for (Eigen::Index i = 0; i < n; ++i) {
    // Calculate non-identity column in B^(-1)
    b = M_inv * (A(all, i) - M(all, i));
    for (Eigen::Index j = 0; j < n; ++j) {
      if (i == j) {
        B_inv_col(j) = 1.0 / (1.0 + b(i));
      } else {
        B_inv_col(j) = -b(j) / (1.0 + b(i));
      }
    }

    // Optimized matrix multiplication: B^(-1) * M^(-1)
    active_row = M_inv(i, all);
    // #pragma omp parallel for
    for (Eigen::Index row = 0; row < n; ++row) {
      if (row == i) {
        for (Eigen::Index col = 0; col < n; ++col) {
          M_inv(row, col) = B_inv_col(row) * active_row(col);
        }
      } else {
        for (Eigen::Index col = 0; col < n; ++col) {
          M_inv(row, col) += B_inv_col(row) * active_row(col);
        }
      }
    }

    // Update matrix M -> should be equal to A at the end
    M(all, i) = A(all, i);
  }

  return M_inv;
}

auto main() -> int {
  using Type      = RT::RecordType<double>;
  using DecayType = RT::decay_record_type_t<Type>;

  constexpr Eigen::Index n = 5;

  Eigen::MatrixX<DecayType> decay_mat = generate_random_spd_matrix<DecayType>(n);
  auto graph                          = std::make_shared<RT::Graph<DecayType>>();
  Eigen::MatrixX<Type> mat(n, n);
  for (Eigen::Index i = 0; i < n; ++i) {
    for (Eigen::Index j = 0; j < n; ++j) {
      mat(i, j) = decay_mat(i, j);
      mat(i, j).register_graph(graph);
    }
  }

  auto mat_inv = dynamic_matrix_inverse(mat);

  std::cout << "Matrix size: " << n << 'x' << n << '\n';
  std::cout << "Number of operations (ADD, MUL, SQRT): "
            << graph->count_ops({RT::NodeType::ADD, RT::NodeType::MUL, RT::NodeType::SQRT}) << '\n';
  std::cout << "  Number ADD:  " << graph->count_op(RT::NodeType::ADD) << '\n';
  std::cout << "  Number MUL:  " << graph->count_op(RT::NodeType::MUL) << '\n';
  std::cout << "  Number SQRT: " << graph->count_op(RT::NodeType::SQRT) << '\n';

  RT::GraphToDotOptions opt{
      .unique_literals      = true,
      .number_only_literals = true,
      .print_node_id        = false,
      .use_op_symbols       = false,
  };

  // Check correctness
  {
    Eigen::MatrixX<DecayType> decay_mat_inv(n, n);
    for (Eigen::Index i = 0; i < n; ++i) {
      for (Eigen::Index j = 0; j < n; ++j) {
        decay_mat_inv(i, j) = mat_inv(i, j).value();
      }
    }
    std::cout << "Correct inverse: " << std::boolalpha
              << is_identity(static_cast<Eigen::MatrixX<DecayType>>(decay_mat * decay_mat_inv))
              << '\n';
  }

  save_to_dot(__FILE__, graph.get(), opt);
}
