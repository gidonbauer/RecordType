#ifndef RT_RECORD_TYPE_HPP_
#define RT_RECORD_TYPE_HPP_

#include <cstdint>
#include <iosfwd>
#ifndef RT_ONLY_FUNDAMENTAL
#include <cmath>
#endif  // RT_ONLY_FUNDAMENTAL

#include "Graph.hpp"
#include "Helper.hpp"
#include "Macros.hpp"
#include "NodeType.hpp"

namespace RT {

template <typename>
class Graph;

template <typename PassiveType>
class RecordType {
  static Graph<PassiveType> s_graph;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  PassiveType m_value;
  uint64_t m_id;
  NodeType m_node_type;

  // Private constructor, allows to choose node type; does not write to graph
  constexpr RecordType(PassiveType value, NodeType node_type) noexcept
      : m_value(std::move(value)),
        m_id(get_unique_id<PassiveType>()),
        m_node_type(node_type) {}

 public:
  // Public default constructor; TODO: Should this write to graph?
  constexpr RecordType() noexcept
      : RecordType(static_cast<PassiveType>(0)) {}

  // Public constructor, gets value
  constexpr RecordType(PassiveType value) noexcept
      : m_value(std::move(value)),
        m_id(get_unique_id<PassiveType>()),
        m_node_type(NodeType::VAR) {
    s_graph.add_operation(m_id, m_node_type, m_value);
  }

  // Copy constructor; TODO: This might be skipped by the compiler
  constexpr RecordType(const RecordType<PassiveType>& other) noexcept
      : m_value(other.m_value),
        m_id(get_unique_id<PassiveType>()),
        m_node_type(NodeType::VAR) {
    s_graph.add_dependencies(other.id());
    s_graph.add_operation(m_id, m_node_type, m_value);
  }

  // Move constructor; TODO: This might be skipped by the compiler
  constexpr RecordType(RecordType<PassiveType>&& other) noexcept
      : m_value(std::move(other.m_value)),
        m_id(get_unique_id<PassiveType>()),
        m_node_type(NodeType::VAR) {
    s_graph.add_dependencies(other.id());
    s_graph.add_operation(m_id, m_node_type, m_value);
  }

  // Copy assign operator
  constexpr auto operator=(const RecordType<PassiveType>& other) noexcept
      -> RecordType<PassiveType>& {
    // Keep copy of other id in case of self assignment
    const auto other_id = other.id();

    m_value     = other.m_value;
    m_id        = get_unique_id<PassiveType>();
    m_node_type = NodeType::VAR;
    s_graph.add_dependencies(other_id);
    s_graph.add_operation(m_id, m_node_type, m_value);
    return *this;
  }

  // Move assign operator
  constexpr auto operator=(RecordType<PassiveType>&& other) noexcept -> RecordType<PassiveType>& {
    const auto other_id = other.id();

    m_value     = std::move(other.m_value);
    m_id        = get_unique_id<PassiveType>();
    m_node_type = NodeType::VAR;
    s_graph.add_dependencies(other_id);
    s_graph.add_operation(m_id, m_node_type, m_value);
    return *this;
  }

  // Destructor
  constexpr ~RecordType() noexcept = default;

  [[nodiscard]] constexpr auto value() const noexcept -> const PassiveType& { return m_value; }
  [[nodiscard]] constexpr auto id() const noexcept -> uint64_t { return m_id; }
  [[nodiscard]] constexpr auto node_type() const noexcept -> NodeType { return m_node_type; }
  [[nodiscard]] static constexpr auto graph() noexcept -> const Graph<PassiveType>& {
    return s_graph;
  }

  [[nodiscard]] constexpr auto operator==(const RecordType<PassiveType>& other) const noexcept
      -> bool {
    return m_value == other.m_value;
  }

  [[nodiscard]] constexpr auto operator<=>(const RecordType<PassiveType>& other) const noexcept {
    return m_value <=> other.m_value;
  }

  constexpr auto operator+=(const RecordType<PassiveType>& to_add) noexcept
      -> RecordType<PassiveType>& {
    *this = *this + to_add;
    return *this;
  }

  constexpr auto operator-=(const RecordType<PassiveType>& to_sub) noexcept
      -> RecordType<PassiveType>& {
    *this = *this - to_sub;
    return *this;
  }

  constexpr auto operator*=(const RecordType<PassiveType>& to_mul) noexcept
      -> RecordType<PassiveType>& {
    *this = *this * to_mul;
    return *this;
  }

  constexpr auto operator/=(const RecordType<PassiveType>& to_div) noexcept
      -> RecordType<PassiveType>& {
    *this = *this / to_div;
    return *this;
  }

  [[nodiscard]] friend constexpr auto operator+(const RecordType<PassiveType>& lhs,
                                                const RecordType<PassiveType>& rhs) noexcept
      -> RecordType<PassiveType> {
    RecordType<PassiveType> res(lhs.value() + rhs.value(), NodeType::ADD);
    s_graph.add_dependencies(lhs.id(), rhs.id());
    s_graph.add_operation(res.id(), res.node_type(), res.value());
    return res;
  }

  [[nodiscard]] friend constexpr auto operator*(const RecordType<PassiveType>& lhs,
                                                const RecordType<PassiveType>& rhs) noexcept
      -> RecordType<PassiveType> {
    RecordType<PassiveType> res(lhs.value() * rhs.value(), NodeType::MUL);
    s_graph.add_dependencies(lhs.id(), rhs.id());
    s_graph.add_operation(res.id(), res.node_type(), res.value());
    return res;
  }

  // TODO: This does not work for integer types
  [[nodiscard]] constexpr auto invert() const noexcept -> RecordType<PassiveType> {
    static_assert(std::is_floating_point_v<PassiveType>,
                  "`PassiveType` has to be a floating point type, otherwise the result would not "
                  "be the same as if we would be using just `PassiveType`");
    RecordType<PassiveType> res(static_cast<PassiveType>(1) / m_value, NodeType::INV);
    s_graph.add_dependencies(id());
    s_graph.add_operation(res.id(), res.node_type(), res.value());
    return res;
  }

  // TODO: This does not work for integer types because of `invert`
  [[nodiscard]] friend constexpr auto operator/(const RecordType<PassiveType>& lhs,
                                                const RecordType<PassiveType>& rhs) noexcept
      -> RecordType<PassiveType> {
    return lhs * rhs.invert();
  }

  // TODO: This does not work for unsigned integer types
  [[nodiscard]] constexpr auto operator-() const noexcept -> RecordType<PassiveType> {
    static_assert(std::is_signed_v<PassiveType>,
                  "`PassiveType` has to be signed, otherwise the result would not be the same as "
                  "if we would be using just `PassiveType`");
    RecordType<PassiveType> res(-m_value, NodeType::NEG);
    s_graph.add_dependencies(id());
    s_graph.add_operation(res.id(), res.node_type(), res.value());
    return res;
  }

  // TODO: This does not work for unsigned integer types
  [[nodiscard]] friend constexpr auto operator-(const RecordType<PassiveType>& lhs,
                                                const RecordType<PassiveType>& rhs) noexcept
      -> RecordType<PassiveType> {
    return lhs + -rhs;
  }

#ifndef RT_ONLY_FUNDAMENTAL
  [[nodiscard]] friend auto sqrt(const RecordType<PassiveType>& x) noexcept
      -> RecordType<PassiveType> {
    RecordType<PassiveType> res(static_cast<PassiveType>(std::sqrt(x.m_value)), NodeType::SQRT);
    s_graph.add_dependencies(x.id());
    s_graph.add_operation(res.id(), res.node_type(), res.value());
    return res;
  }

  [[nodiscard]] friend auto sin(const RecordType<PassiveType>& x) noexcept
      -> RecordType<PassiveType> {
    RecordType<PassiveType> res(static_cast<PassiveType>(std::sin(x.m_value)), NodeType::SIN);
    s_graph.add_dependencies(x.id());
    s_graph.add_operation(res.id(), res.node_type(), res.value());
    return res;
  }

  [[nodiscard]] friend auto cos(const RecordType<PassiveType>& x) noexcept
      -> RecordType<PassiveType> {
    RecordType<PassiveType> res(static_cast<PassiveType>(std::cos(x.m_value)), NodeType::COS);
    s_graph.add_dependencies(x.id());
    s_graph.add_operation(res.id(), res.node_type(), res.value());
    return res;
  }
#else
  [[noreturn]] friend auto sqrt(const RecordType<PassiveType>& /*x*/) noexcept
      -> RecordType<PassiveType> {
    RT_PANIC("Operation `sqrt` is not allowed becaue the macro `RT_ONLY_FUNDAMENTAL` is defined.");
  }

  [[noreturn]] friend auto sin(const RecordType<PassiveType>& /*x*/) noexcept
      -> RecordType<PassiveType> {
    RT_PANIC("Operation `sin` is not allowed becaue the macro `RT_ONLY_FUNDAMENTAL` is defined.");
  }

  [[noreturn]] friend auto cos(const RecordType<PassiveType>& /*x*/) noexcept
      -> RecordType<PassiveType> {
    RT_PANIC("Operation `cos` is not allowed becaue the macro `RT_ONLY_FUNDAMENTAL` is defined.");
  }
#endif  // RT_ONLY_FUNDAMENTAL
};
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
template <typename PassiveType>
Graph<PassiveType> RecordType<PassiveType>::s_graph = Graph<PassiveType>();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

template <typename PassiveType>
auto operator<<(std::ostream& out, const RecordType<PassiveType>& t) noexcept -> std::ostream& {
  out << "node_" << t.id() << " (" << t.node_type() << ", " << t.value() << ")";
  return out;
}

}  // namespace RT

// NOLINTBEGIN(cert-dcl58-cpp)
namespace std {

template <typename PassiveType>
[[nodiscard]] auto sqrt(const RT::RecordType<PassiveType>& x) noexcept
    -> RT::RecordType<PassiveType> {
  return sqrt(x);
}

template <typename PassiveType>
[[nodiscard]] auto sin(const RT::RecordType<PassiveType>& x) noexcept
    -> RT::RecordType<PassiveType> {
  return sin(x);
}

template <typename PassiveType>
[[nodiscard]] auto cos(const RT::RecordType<PassiveType>& x) noexcept
    -> RT::RecordType<PassiveType> {
  return cos(x);
}

}  // namespace std
// NOLINTEND(cert-dcl58-cpp)

#endif  // RT_RECORD_TYPE_HPP_
