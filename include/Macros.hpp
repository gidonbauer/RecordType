#ifndef RT_MACROS_HPP_
#define RT_MACROS_HPP_

#include <filesystem>
#include <iostream>

#if __has_include(<source_location>)
#include <source_location>
#endif

#if __cpp_lib_source_location >= 201907L

[[nodiscard]] inline auto
RT_ERROR_LOC(const std::source_location loc = std::source_location::current()) -> std::string {
  using namespace std::literals;
  return "`"s + std::string{loc.function_name()} + "` (\033[95m"s +
         std::string{std::filesystem::path(loc.file_name()).filename()} + ":"s +
         std::to_string(loc.line()) + ":"s + std::to_string(loc.column()) + "\033[0m)"s;
}

#else

// NOLINTBEGIN(cppcoreguidelines-macro-usage,cppcoreguidelines-pro-bounds-array-to-pointer-decay)
#define RT_ERROR_LOC()                                                                             \
  ("`" + std::string(__func__) + "` (\033[95m" +                                                   \
   std::string(std::filesystem::path(__FILE__).filename()) + ":" + std::to_string(__LINE__) +      \
   "\033[0m)")
// NOLINTEND(cppcoreguidelines-macro-usage,cppcoreguidelines-pro-bounds-array-to-pointer-decay)

#endif

// NOLINTBEGIN(cppcoreguidelines-macro-usage,cppcoreguidelines-pro-bounds-array-to-pointer-decay,bugprone-macro-parentheses)
#define RT_TODO(msg)                                                                               \
  do {                                                                                             \
    std::cerr << "\033[35m[TODO]\033[0m " << RT_ERROR_LOC() << ": " << msg << "\n";                \
    std::exit(2);                                                                                  \
  } while (false)

#define RT_PANIC(msg)                                                                              \
  do {                                                                                             \
    std::cerr << "\033[31m[ERROR]\033[0m " << RT_ERROR_LOC() << ": " << msg << "\n";               \
    std::exit(1);                                                                                  \
  } while (false)

#define RT_DEBUG_PRINT(x) std::cerr << "\033[94m[DEBUG]\033[0m " << #x << " = " << (x) << '\n'

#define RT_ASSERT(condition, msg)                                                                  \
  do {                                                                                             \
    if (!(condition)) {                                                                            \
      std::cerr << "\033[31m[ASSERT]\033[0m " << RT_ERROR_LOC() << ": `" << #condition             \
                << "` failed: " << msg << '\n';                                                    \
      std::abort();                                                                                \
    }                                                                                              \
  } while (false)
// NOLINTEND(cppcoreguidelines-macro-usage,cppcoreguidelines-pro-bounds-array-to-pointer-decay,bugprone-macro-parentheses)

#endif  // RT_MACROS_HPP_
