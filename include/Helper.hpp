#ifndef RT_HELPER_HPP_
#define RT_HELPER_HPP_

#include <atomic>
#include <cstdint>
#include <cxxabi.h>
#include <string>
#include <type_traits>

namespace RT {

template <typename PassiveType>
[[nodiscard]] auto get_unique_id() noexcept -> uint64_t {
  static std::atomic<uint64_t> id_counter = 0ull;
  return id_counter++;
}

template <typename T>
[[nodiscard]] auto type_name() -> std::string {
  using namespace std::string_literals;

  int status;
  char* name_cstr = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);

  if (status != 0 || name_cstr == nullptr) {
    std::free(name_cstr);  // NOLINT(cppcoreguidelines-owning-memory,cppcoreguidelines-no-malloc)
    switch (status) {
      case -1:
        throw std::runtime_error(
            "Demagleing failed with status -1: A memory allocation failure occurred.");
      case -2:
        throw std::runtime_error("Demagleing failed with status -2: mangled_name is not a valid "
                                 "name under the C++ ABI mangling rules.");
      case -3:
        throw std::runtime_error(
            "Demagleing failed with status -3: One of the arguments is invalid.");
      default:
        throw std::runtime_error("Demagleing failed with unknown status "s +
                                 std::to_string(status) + "."s);
    }
  }

  std::string name(name_cstr);
  std::free(name_cstr);  // NOLINT(cppcoreguidelines-owning-memory,cppcoreguidelines-no-malloc)

  if (std::is_volatile_v<std::remove_reference_t<T>>) {
    name += " volatile"s;
  }
  if (std::is_const_v<std::remove_reference_t<T>>) {
    name += " const"s;
  }
  if (std::is_lvalue_reference_v<T>) {
    name += "&"s;
  }
  if (std::is_rvalue_reference_v<T>) {
    name += "&&"s;
  }

  return name;
}

}  // namespace RT

#endif  // RT_HELPER_HPP_
