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

template <typename ActiveType>
class Graph;

template <typename ActiveType>
class RecordType {
  static Graph<ActiveType> s_graph;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

  ActiveType m_value;
  uint64_t m_id;
  NodeType m_node_type;

  // Private constructor, allows to choose node type; does not write to graph
  constexpr RecordType(ActiveType value, NodeType node_type) noexcept
      : m_value(std::move(value)),
        m_id(get_unique_id()),
        m_node_type(node_type) {}

 public:
  // Public default constructor; TODO: Should this write to graph?
  constexpr RecordType() noexcept
      : RecordType(static_cast<ActiveType>(0), NodeType::VAR) {}

  // Public constructor, gets value
  constexpr RecordType(ActiveType value) noexcept
      : m_value(std::move(value)),
        m_id(get_unique_id()),
        m_node_type(NodeType::VAR) {
    s_graph.insert_node_edge(RecordType<ActiveType>(value, NodeType::LITERAL), *this);
  }

  // Copy constructor; TODO: This might be skipped by the compiler
  constexpr RecordType(const RecordType<ActiveType>& other) noexcept
      : m_value(other.m_value),
        m_id(get_unique_id()),
        m_node_type(NodeType::VAR) {
    s_graph.insert_node_edge(other, *this);
  }

  // Move constructor; TODO: This might be skipped by the compiler
  constexpr RecordType(RecordType<ActiveType>&& other) noexcept
      : m_value(std::move(other.m_value)),
        m_id(get_unique_id()),
        m_node_type(NodeType::VAR) {
    s_graph.insert_node_edge(other, *this);
  }

  // Copy assign operator
  constexpr auto operator=(const RecordType<ActiveType>& other) noexcept
      -> RecordType<ActiveType>& {
    m_value     = other.m_value;
    m_id        = get_unique_id();
    m_node_type = NodeType::VAR;
    s_graph.insert_node_edge(other, *this);
    return *this;
  }

  // Move assign operator
  constexpr auto operator=(RecordType<ActiveType>&& other) noexcept -> RecordType<ActiveType>& {
    m_value     = std::move(other.m_value);
    m_id        = get_unique_id();
    m_node_type = NodeType::VAR;
    s_graph.insert_node_edge(other, *this);
    return *this;
  }

  // Destructor
  constexpr ~RecordType() noexcept = default;

  [[nodiscard]] constexpr auto value() const noexcept -> const ActiveType& { return m_value; }
  [[nodiscard]] constexpr auto id() const noexcept -> uint64_t { return m_id; }
  [[nodiscard]] constexpr auto node_type() const noexcept -> NodeType { return m_node_type; }
  [[nodiscard]] static constexpr auto graph() noexcept -> const Graph<ActiveType>& {
    return s_graph;
  }

  [[nodiscard]] constexpr auto operator==(const RecordType<ActiveType>& other) const noexcept
      -> bool {
    return m_value == other.m_value;
  }

  [[nodiscard]] constexpr auto operator<=>(const RecordType<ActiveType>& other) const noexcept {
    return m_value <=> other.m_value;
  }

  constexpr auto operator+=(const RecordType<ActiveType>& to_add) noexcept
      -> RecordType<ActiveType>& {
    *this = *this + to_add;
    return *this;
  }

  constexpr auto operator-=(const RecordType<ActiveType>& to_sub) noexcept
      -> RecordType<ActiveType>& {
    *this = *this - to_sub;
    return *this;
  }

  constexpr auto operator*=(const RecordType<ActiveType>& to_mul) noexcept
      -> RecordType<ActiveType>& {
    *this = *this * to_mul;
    return *this;
  }

  constexpr auto operator/=(const RecordType<ActiveType>& to_div) noexcept
      -> RecordType<ActiveType>& {
    *this = *this / to_div;
    return *this;
  }

  [[nodiscard]] friend constexpr auto operator+(const RecordType<ActiveType>& lhs,
                                                const RecordType<ActiveType>& rhs) noexcept
      -> RecordType<ActiveType> {
    RecordType<ActiveType> res(lhs.value() + rhs.value(), NodeType::ADD);
    s_graph.insert_node_edge(lhs, res);
    s_graph.insert_node_edge(rhs, res);
    return res;
  }

  [[nodiscard]] friend constexpr auto operator*(const RecordType<ActiveType>& lhs,
                                                const RecordType<ActiveType>& rhs) noexcept
      -> RecordType<ActiveType> {
    RecordType<ActiveType> res(lhs.value() * rhs.value(), NodeType::MUL);
    s_graph.insert_node_edge(lhs, res);
    s_graph.insert_node_edge(rhs, res);
    return res;
  }

  // TODO: This does not work for integer types
  [[nodiscard]] constexpr auto invert() const noexcept -> RecordType<ActiveType> {
    RecordType<ActiveType> res(static_cast<ActiveType>(1) / m_value, NodeType::INV);
    s_graph.insert_node_edge(*this, res);
    return res;
  }

  // TODO: This does not work for integer types because of `invert`
  [[nodiscard]] friend constexpr auto operator/(const RecordType<ActiveType>& lhs,
                                                const RecordType<ActiveType>& rhs) noexcept
      -> RecordType<ActiveType> {
    return lhs * rhs.invert();
  }

  // TODO: This does not work for unsigned integer types
  [[nodiscard]] constexpr auto operator-() const noexcept -> RecordType<ActiveType> {
    RecordType<ActiveType> res(-m_value, NodeType::NEG);
    s_graph.insert_node_edge(*this, res);
    return res;
  }

  // TODO: This does not work for unsigned integer types
  [[nodiscard]] friend constexpr auto operator-(const RecordType<ActiveType>& lhs,
                                                const RecordType<ActiveType>& rhs) noexcept
      -> RecordType<ActiveType> {
    return lhs + -rhs;
  }

#ifndef RT_ONLY_FUNDAMENTAL
  [[nodiscard]] friend auto sqrt(const RecordType<ActiveType>& x) noexcept
      -> RecordType<ActiveType> {
    RecordType<ActiveType> res(static_cast<ActiveType>(std::sqrt(x.m_value)), NodeType::SQRT);
    s_graph.insert_node_edge(x, res);
    return res;
  }
#else
  [[noreturn]] friend auto sqrt(const RecordType<ActiveType>& /*x*/) noexcept
      -> RecordType<ActiveType> {
    RT_PANIC("Operation `sqrt` is not allowed becaue the macro `RT_ONLY_FUNDAMENTAL` is defined.");
  }
#endif  // RT_ONLY_FUNDAMENTAL
};
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
template <typename ActiveType>
Graph<ActiveType> RecordType<ActiveType>::s_graph = Graph<ActiveType>();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

template <typename ActiveType>
auto operator<<(std::ostream& out, const RecordType<ActiveType>& t) noexcept -> std::ostream& {
  out << "node_" << t.id() << " (" << t.node_type() << ", " << t.value() << ")";
  return out;
}

}  // namespace RT

// NOLINTBEGIN(cert-dcl58-cpp)
namespace std {

template <typename ActiveType>
[[nodiscard]] auto sqrt(const RT::RecordType<ActiveType>& x) noexcept
    -> RT::RecordType<ActiveType> {
  return sqrt(x);
}

}  // namespace std
// NOLINTEND(cert-dcl58-cpp)

#endif  // RT_RECORD_TYPE_HPP_
