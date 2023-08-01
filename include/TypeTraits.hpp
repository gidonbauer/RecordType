#ifndef RT_TYPE_TRAITS_HPP_
#define RT_TYPE_TRAITS_HPP_

#include <type_traits>

namespace RT {

template <typename PassiveType>
class RecordType;

// - Check for RecordType --------------------------------------------------------------------------
template <typename T>
struct is_record_type : std::false_type {
  using underlying_type = T;
};

template <typename T>
struct is_record_type<RecordType<T>> : std::true_type {
  using underlying_type = T;
};

template <typename T>
constexpr bool is_record_type_v = is_record_type<T>::value;

// - Decay RecordType to underlying type -----------------------------------------------------------
template <typename T>
using decay_record_type_t = typename is_record_type<T>::underlying_type;

// - Container concept -----------------------------------------------------------------------------
template <typename Container>
concept FwdContainerType = requires(Container c) {
  std::begin(c);
  std::end(c);

  std::cbegin(c);
  std::cend(c);
};

template <typename Container>
concept BidirContainerType = requires(Container c) {
  std::begin(c);
  std::end(c);

  std::cbegin(c);
  std::cend(c);

  std::rbegin(c);
  std::rend(c);

  std::crbegin(c);
  std::crend(c);
};

}  // namespace RT

#endif  // RT_TYPE_TRAITS_HPP_