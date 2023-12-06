#include <iostream>
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

  Eigen::MatrixX<Type> mat_inv = mat.llt().solve(Eigen::MatrixX<Type>::Identity(n, n));

  std::cout << "Matrix size: " << n << 'x' << n << '\n';
  std::cout << "Number of operations (ADD, MUL, SQRT): "
            << graph->count_ops({RT::NodeType::ADD, RT::NodeType::MUL, RT::NodeType::SQRT}) << '\n';
  std::cout << "  Number ADD:  " << graph->count_op(RT::NodeType::ADD) << '\n';
  std::cout << "  Number MUL:  " << graph->count_op(RT::NodeType::MUL) << '\n';
  std::cout << "  Number SQRT: " << graph->count_op(RT::NodeType::SQRT) << '\n';

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

  RT::GraphToDotOptions opt{
      .unique_literals      = true,
      .number_only_literals = true,
      .print_node_id        = false,
      .use_op_symbols       = false,
  };
  save_to_dot(__FILE__, graph.get(), opt);
}
