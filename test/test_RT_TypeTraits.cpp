#include <gtest/gtest.h>

#include "TypeTraits.hpp"

#define EXPECT_NOT_RECORD_TYPE(T)                                                                  \
  EXPECT_FALSE(RT::is_record_type_v<T>)                                                            \
      << "`" #T "` is not a RecordType but `RT::is_record_type_v<" #T ">` evaluated to true.";

#define EXPECT_RECORD_TYPE(T)                                                                      \
  EXPECT_TRUE(RT::is_record_type_v<T>)                                                             \
      << "`" #T "` is a RecordType but `RT::is_record_type_v<" #T ">` evaluated to false.";

#define EXPECT_DECAY_TO(T, Decay)                                                                  \
  do {                                                                                             \
    constexpr bool condition = std::is_same_v<RT::decay_record_type_t<T>, Decay>;                  \
    EXPECT_TRUE(condition) << "`" #T "` should decay to `" #Decay "` but decays to `"              \
                           << RT::type_name<RT::decay_record_type_t<T>>() << "`.";                 \
  } while (false)

TEST(test_RT_TypeTraits, FundamentalTypes) {
  EXPECT_NOT_RECORD_TYPE(int);
  EXPECT_NOT_RECORD_TYPE(float);
  EXPECT_NOT_RECORD_TYPE(double);

  EXPECT_DECAY_TO(int, int);
  EXPECT_DECAY_TO(float, float);
  EXPECT_DECAY_TO(double, double);
}

TEST(test_RT_TypeTraits, RecordTypes) {
  EXPECT_RECORD_TYPE(RT::RecordType<int>);
  EXPECT_RECORD_TYPE(RT::RecordType<float>);
  EXPECT_RECORD_TYPE(RT::RecordType<double>);

  EXPECT_DECAY_TO(RT::RecordType<int>, int);
  EXPECT_DECAY_TO(RT::RecordType<float>, float);
  EXPECT_DECAY_TO(RT::RecordType<double>, double);
}

TEST(test_RT_TypeTraits, ContainerTypes) {
  EXPECT_NOT_RECORD_TYPE(std::vector<int>);
  EXPECT_NOT_RECORD_TYPE(std::vector<float>);
  EXPECT_NOT_RECORD_TYPE(std::vector<double>);

  EXPECT_NOT_RECORD_TYPE(std::vector<RT::RecordType<int>>);
  EXPECT_NOT_RECORD_TYPE(std::vector<RT::RecordType<float>>);
  EXPECT_NOT_RECORD_TYPE(std::vector<RT::RecordType<double>>);

  EXPECT_DECAY_TO(std::vector<int>, std::vector<int>);
  EXPECT_DECAY_TO(std::vector<float>, std::vector<float>);
  EXPECT_DECAY_TO(std::vector<double>, std::vector<double>);

  EXPECT_DECAY_TO(std::vector<RT::RecordType<int>>, std::vector<RT::RecordType<int>>);
  EXPECT_DECAY_TO(std::vector<RT::RecordType<float>>, std::vector<RT::RecordType<float>>);
  EXPECT_DECAY_TO(std::vector<RT::RecordType<double>>, std::vector<RT::RecordType<double>>);
}

TEST(test_RT_TypeTraits, TypeName) {
  using namespace std::string_literals;

  EXPECT_EQ(RT::type_name<int>(), "int"s);
  EXPECT_EQ(RT::type_name<float>(), "float"s);
  EXPECT_EQ(RT::type_name<double>(), "double"s);

  EXPECT_EQ(RT::type_name<RT::RecordType<int>>(), "RT::RecordType<int>"s);
  EXPECT_EQ(RT::type_name<RT::RecordType<float>>(), "RT::RecordType<float>"s);
  EXPECT_EQ(RT::type_name<RT::RecordType<double>>(), "RT::RecordType<double>"s);

  EXPECT_EQ(RT::type_name<int*>(), "int*"s);
  EXPECT_EQ(RT::type_name<const int*>(), "int const*"s);
  EXPECT_EQ(RT::type_name<int const* const>(), "int const* const"s);

  EXPECT_EQ(RT::type_name<volatile int*>(), "int volatile*"s);
  EXPECT_EQ(RT::type_name<int volatile* volatile>(), "int volatile* volatile"s);
  EXPECT_EQ(RT::type_name<int volatile* const volatile>(), "int volatile* volatile const"s);

  EXPECT_EQ(RT::type_name<int**>(), "int**"s);
  EXPECT_EQ(RT::type_name<const int**>(), "int const**"s);
  EXPECT_EQ(RT::type_name<const int** const>(), "int const** const"s);

  EXPECT_EQ(RT::type_name<const int>(), "int const"s);
  EXPECT_EQ(RT::type_name<int&>(), "int&"s);
  EXPECT_EQ(RT::type_name<const int&>(), "int const&"s);

  EXPECT_EQ(RT::type_name<int&&>(), "int&&"s);
  EXPECT_EQ(RT::type_name<const int&&>(), "int const&&"s);
}
