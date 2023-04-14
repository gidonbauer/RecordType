#ifndef RT_TYPE_TRAITS_HPP_
#define RT_TYPE_TRAITS_HPP_

#include <type_traits>

#include "Macros.hpp"
#include "RecordType.hpp"

namespace RT {

// - Check for RecordType --------------------------------------------------------------------------
template <typename>
struct is_record_type : std::false_type {
  using underlying_type = void;
};

template <typename T>
struct is_record_type<RecordType<T>> : std::true_type {
  using underlying_type = T;
};

template <typename T>
constexpr bool is_record_type_v = is_record_type<T>::value;

// - Decay RecordType to underlying type -----------------------------------------------------------
namespace internal {

template <typename T>
[[noreturn]] constexpr auto decay_record_type() -> T {
  RT_PANIC("This function should never be called. Use the type `decay_record_type`.");
}

template <typename T>
requires is_record_type_v<T>
[[noreturn]] constexpr auto decay_record_type() -> typename is_record_type<T>::underlying_type {
  RT_PANIC("This function should never be called. Use the type `decay_record_type`.");
}

}  // namespace internal

template <typename T>
using decay_record_type_t = decltype(internal::decay_record_type<T>());

}  // namespace RT

#endif  // RT_TYPE_TRAITS_HPP_