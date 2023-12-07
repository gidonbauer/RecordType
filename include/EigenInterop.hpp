#ifndef RT_EIGEN_INTEROP_HPP_
#define RT_EIGEN_INTEROP_HPP_

#include <Eigen/Core>

#include "RecordType.hpp"

namespace Eigen {

template <typename PassiveType>
struct NumTraits<RT::RecordType<PassiveType>> : NumTraits<PassiveType> {
  using Real       = RT::RecordType<PassiveType>;
  using NonInteger = RT::RecordType<PassiveType>;
  using Nested     = RT::RecordType<PassiveType>;

  enum {
    IsComplex             = NumTraits<PassiveType>::IsComplex,
    IsInteger             = NumTraits<PassiveType>::IsInteger,
    IsSigned              = NumTraits<PassiveType>::IsSigned,
    RequireInitialization = NumTraits<PassiveType>::RequireInitialization,
    ReadCost              = NumTraits<PassiveType>::ReadCost,
    AddCost               = NumTraits<PassiveType>::AddCost,
    MulCost               = NumTraits<PassiveType>::MulCost,
  };
};

template <typename BinaryOp, typename PassiveType>
struct ScalarBinaryOpTraits<RT::RecordType<PassiveType>, PassiveType, BinaryOp> {
  using ReturnType = RT::RecordType<PassiveType>;
};

template <typename BinaryOp, typename PassiveType>
struct ScalarBinaryOpTraits<PassiveType, RT::RecordType<PassiveType>, BinaryOp> {
  using ReturnType = RT::RecordType<PassiveType>;
};

}  // namespace Eigen

#endif  // RT_EIGEN_INTEROP_HPP_
