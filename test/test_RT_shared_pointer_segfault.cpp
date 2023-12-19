#include <gtest/gtest.h>

#include <Eigen/Dense>

#include "EigenInterop.hpp"
#include "RecordType.hpp"

TEST(test_shared_pointer_segfault, dynamic_eigen_random) {
  using Passive_t = double;
  using Rec_t     = RT::RecordType<Passive_t>;

  constexpr int N = 100;

  Eigen::MatrixX<Rec_t> mat = Eigen::MatrixX<Passive_t>::Random(N, N);
}

TEST(test_shared_pointer_segfault, static_eigen_random) {
  using Passive_t = double;
  using Rec_t     = RT::RecordType<Passive_t>;

  constexpr int N                = 10;
  Eigen::Matrix<Rec_t, N, N> mat = Eigen::Matrix<Passive_t, N, N>::Random(N, N);
}
