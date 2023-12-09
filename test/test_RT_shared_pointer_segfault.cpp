#include <gtest/gtest.h>

#include <vector>

#include <Eigen/Dense>

#include "EigenInterop.hpp"
#include "RecordType.hpp"

TEST(test_shared_pointer_segfault, uninitialized_data) {
  using Passive_t = double;
  using Rec_t     = RT::RecordType<Passive_t>;

  Rec_t* rec = static_cast<Rec_t*>(std::malloc(sizeof(Rec_t)));
  new (rec) Rec_t();  // Dirty hack!!!
  *rec = 3.14;
  delete rec;
}

/*
  Dynamic eigen matricies leave entries uninitialized. If we then assign a value to those
  uninitialized entries, we try to read the reference-count of the shared pointer from a random
  location, because the pointer was not properly initialized to a nullptr. This leads to a segfault.
    -> Workaround: Use Eigen::Map or static Eigen::Matrix
*/
TEST(test_shared_pointer_segfault, dynamic_eigen_matrix_workaround_map) {
  using Passive_t = double;
  using Rec_t     = RT::RecordType<Passive_t>;

  constexpr int N = 100;

  std::vector<Rec_t> data(N * N);
  Eigen::Map<Eigen::Matrix<Rec_t, N, N>> mat(data.data(), N, N);
  mat = Eigen::MatrixX<Passive_t>::Random(N, N);
}

TEST(test_shared_pointer_segfault, dynamic_eigen_matrix_workaround_static) {
  using Passive_t = double;
  using Rec_t     = RT::RecordType<Passive_t>;

  constexpr int N                = 10;
  Eigen::Matrix<Rec_t, N, N> mat = Eigen::MatrixX<Passive_t>::Random(N, N);
}
