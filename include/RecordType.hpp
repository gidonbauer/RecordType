#ifndef RT_RECORD_TYPE_HPP_
#define RT_RECORD_TYPE_HPP_

#include <algorithm>
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <utility>
#ifndef RT_ONLY_FUNDAMENTAL
#include <cmath>
#endif  // RT_ONLY_FUNDAMENTAL

#include "Definitions.hpp"
#include "Graph.hpp"
#include "Helper.hpp"
#include "Macros.hpp"
#include "NodeType.hpp"
#include "TypeTraits.hpp"

namespace RT {

template <typename>
class Graph;

template <typename PassiveType>
class RecordType {
  mutable std::shared_ptr<Graph<PassiveType>> m_graph{};
  PassiveType m_value{};
  mutable id_t m_id{};
  mutable NodeType m_node_type{};

  // Private constructor, allows to choose node type; does not write to graph
  constexpr RecordType(PassiveType value, NodeType node_type) noexcept
      : m_value(std::move(value)),
        m_id(UNREGISTERED),
        m_node_type(node_type) {}

 public:
  // Public default constructor
  constexpr RecordType() noexcept
      : RecordType(static_cast<PassiveType>(0)) {}

  // Public constructor, gets value
  constexpr RecordType(PassiveType value) noexcept
      : m_value(std::move(value)),
        m_id(UNREGISTERED),
        m_node_type(NodeType::VAR) {}

  // Copy constructor
  constexpr RecordType(const RecordType<PassiveType>& other) noexcept
      : m_graph(other.m_graph),
        m_value(other.m_value),
        m_id(UNREGISTERED),
        m_node_type(NodeType::VAR) {
    if (m_graph) {
      m_id = m_graph->add_operation(m_node_type, m_value, {other.id()});
    }
  }

  // Move constructor
  constexpr RecordType(RecordType<PassiveType>&& other) noexcept
      : m_graph(std::exchange(other.m_graph, nullptr)),
        m_value(std::move(other.m_value)),
        m_id(UNREGISTERED),
        m_node_type(NodeType::VAR) {
    if (m_graph) {
      m_id = m_graph->add_operation(m_node_type, m_value, {other.id()});
    }
  }

  // Copy assign operator
  constexpr auto operator=(const RecordType<PassiveType>& other) noexcept
      -> RecordType<PassiveType>& {
    // Keep copy of other id in case of self assignment
    auto other_id = other.id();

    m_graph     = get_graph(m_graph, other.m_graph);
    m_value     = other.m_value;
    m_node_type = NodeType::VAR;
    if (m_graph) {
      if (other.id() == UNREGISTERED) {
        other.m_id = m_graph->add_operation(other.node_type(), other.value());
        other_id   = other.m_id;
      }

      m_id = m_graph->add_operation(m_node_type, m_value, {other_id});
    }
    return *this;
  }

  // Move assign operator
  constexpr auto operator=(RecordType<PassiveType>&& other) noexcept -> RecordType<PassiveType>& {
    auto other_id = other.id();

    m_graph     = get_graph(m_graph, other.m_graph);
    m_value     = std::move(other.m_value);
    m_node_type = NodeType::VAR;
    if (m_graph) {
      if (other.id() == UNREGISTERED) {
        // TODO: Do we need to override this, shouldn't other be thrown away?
        other.m_id = m_graph->add_operation(other.node_type(), m_value);
        other_id   = other.m_id;
      }

      m_id = m_graph->add_operation(m_node_type, m_value, {other_id});
    }
    return *this;
  }

  // Destructor
  constexpr ~RecordType() noexcept = default;

  // Set graph
  constexpr void register_graph(std::shared_ptr<Graph<PassiveType>> graph) const noexcept {
    m_graph = graph;
    m_id    = m_graph->add_operation(m_node_type, m_value);
    m_graph->mark_input(m_id);
  }

  // Mark variable as output
  constexpr void mark_output() const noexcept {
    RT_ASSERT(m_graph,
              "Variable is not registered in a graph and can not be marked as an output variable.");
    m_graph->mark_output(m_id);
  }

  // Mark variable as output
  constexpr void add_name(const std::string& name) const noexcept {
    RT_ASSERT(m_graph,
              "Variable is not registered in a graph and can not be marked as an output variable.");
    m_graph->add_name(m_id, name);
  }

  [[nodiscard]] constexpr auto value() const noexcept -> const PassiveType& { return m_value; }
  [[nodiscard]] constexpr auto id() const noexcept -> id_t { return m_id; }
  [[nodiscard]] constexpr auto node_type() const noexcept -> NodeType { return m_node_type; }
  [[nodiscard]] constexpr auto graph() const noexcept -> const Graph<PassiveType>* {
    return m_graph.get();
  }

 private:
  [[nodiscard]] static constexpr auto
  get_graph(std::shared_ptr<Graph<PassiveType>> lhs_graph,
            std::shared_ptr<Graph<PassiveType>> rhs_graph) noexcept
      -> std::shared_ptr<Graph<PassiveType>> {
    if (lhs_graph != rhs_graph) {
      if (lhs_graph && rhs_graph) {
        return nullptr;
      } else if (lhs_graph) {
        return lhs_graph;
      } else if (rhs_graph) {
        return rhs_graph;
      } else {
        RT_PANIC("Unreachable.");
      }
    }
    return lhs_graph;
  }

 public:
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

    auto graph = get_graph(lhs.m_graph, rhs.m_graph);
    if (graph) {
      if (lhs.id() == UNREGISTERED) {
        lhs.m_id = graph->add_operation(lhs.node_type(), lhs.value());
      }
      if (rhs.id() == UNREGISTERED) {
        rhs.m_id = graph->add_operation(rhs.node_type(), rhs.value());
      }

      res.m_id    = graph->add_operation(res.node_type(), res.value(), {lhs.id(), rhs.id()});
      res.m_graph = graph;
    }
    return res;
  }

  [[nodiscard]] friend constexpr auto operator*(const RecordType<PassiveType>& lhs,
                                                const RecordType<PassiveType>& rhs) noexcept
      -> RecordType<PassiveType> {
    RecordType<PassiveType> res(lhs.value() * rhs.value(), NodeType::MUL);

    auto graph = get_graph(lhs.m_graph, rhs.m_graph);
    if (graph) {
      if (lhs.id() == UNREGISTERED) {
        lhs.m_id = graph->add_operation(lhs.node_type(), lhs.value());
      }
      if (rhs.id() == UNREGISTERED) {
        rhs.m_id = graph->add_operation(rhs.node_type(), rhs.value());
      }

      res.m_id    = graph->add_operation(res.node_type(), res.value(), {lhs.id(), rhs.id()});
      res.m_graph = graph;
    }
    return res;
  }

  // TODO: This does not work for integer types
  [[nodiscard]] constexpr auto invert() const noexcept -> RecordType<PassiveType> {
    static_assert(std::is_floating_point_v<PassiveType>,
                  "`PassiveType` has to be a floating point type, otherwise the result would not "
                  "be the same as if we would be using just `PassiveType`");
    RecordType<PassiveType> res(static_cast<PassiveType>(1) / m_value, NodeType::INV);
    if (m_graph) {
      res.m_id    = m_graph->add_operation(res.node_type(), res.value(), {id()});
      res.m_graph = m_graph;
    }
    return res;
  }

  // TODO: This does not work for integer types because of `invert`
  [[nodiscard]] friend constexpr auto operator/(const RecordType<PassiveType>& lhs,
                                                const RecordType<PassiveType>& rhs) noexcept
      -> RecordType<PassiveType> {
    RecordType<PassiveType> res(lhs.value() / rhs.value(), NodeType::DIV);

    auto graph = get_graph(lhs.m_graph, rhs.m_graph);
    if (graph) {
      if (lhs.id() == UNREGISTERED) {
        lhs.m_id = graph->add_operation(lhs.node_type(), lhs.value());
      }
      if (rhs.id() == UNREGISTERED) {
        rhs.m_id = graph->add_operation(rhs.node_type(), rhs.value());
      }

      res.m_id    = graph->add_operation(res.node_type(), res.value(), {lhs.id(), rhs.id()});
      res.m_graph = graph;
    }
    return res;
  }

  // TODO: This does not work for unsigned integer types
  [[nodiscard]] constexpr auto operator-() const noexcept -> RecordType<PassiveType> {
    static_assert(std::is_signed_v<PassiveType>,
                  "`PassiveType` has to be signed, otherwise the result would not be the same as "
                  "if we would be using just `PassiveType`");
    RecordType<PassiveType> res(-m_value, NodeType::NEG);
    if (m_graph) {
      res.m_id    = m_graph->add_operation(res.node_type(), res.value(), {id()});
      res.m_graph = m_graph;
    }
    return res;
  }

  // TODO: This does not work for unsigned integer types
  [[nodiscard]] friend constexpr auto operator-(const RecordType<PassiveType>& lhs,
                                                const RecordType<PassiveType>& rhs) noexcept
      -> RecordType<PassiveType> {
    RecordType<PassiveType> res(lhs.value() - rhs.value(), NodeType::SUB);

    auto graph = get_graph(lhs.m_graph, rhs.m_graph);
    if (graph) {
      if (lhs.id() == UNREGISTERED) {
        lhs.m_id = graph->add_operation(lhs.node_type(), lhs.value());
      }
      if (rhs.id() == UNREGISTERED) {
        rhs.m_id = graph->add_operation(rhs.node_type(), rhs.value());
      }

      res.m_id    = graph->add_operation(res.node_type(), res.value(), {lhs.id(), rhs.id()});
      res.m_graph = graph;
    }
    return res;
  }

#ifndef RT_ONLY_FUNDAMENTAL
  [[nodiscard]] friend auto sqrt(const RecordType<PassiveType>& x) noexcept
      -> RecordType<PassiveType> {
    RecordType<PassiveType> res(static_cast<PassiveType>(std::sqrt(x.m_value)), NodeType::SQRT);
    if (x.m_graph) {
      res.m_id    = x.m_graph->add_operation(res.node_type(), res.value(), {x.id()});
      res.m_graph = x.m_graph;
    }
    return res;
  }

  [[nodiscard]] friend auto sin(const RecordType<PassiveType>& x) noexcept
      -> RecordType<PassiveType> {
    RecordType<PassiveType> res(static_cast<PassiveType>(std::sin(x.m_value)), NodeType::SIN);
    if (x.m_graph) {
      res.m_id    = x.m_graph->add_operation(res.node_type(), res.value(), {x.id()});
      res.m_graph = x.m_graph;
    }
    return res;
  }

  [[nodiscard]] friend auto cos(const RecordType<PassiveType>& x) noexcept
      -> RecordType<PassiveType> {
    RecordType<PassiveType> res(static_cast<PassiveType>(std::cos(x.m_value)), NodeType::COS);
    if (x.m_graph) {
      res.m_id    = x.m_graph->add_operation(res.node_type(), res.value(), {x.id()});
      res.m_graph = x.m_graph;
    }
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

template <typename PassiveType>
auto operator<<(std::ostream& out, const RecordType<PassiveType>& t) noexcept -> std::ostream& {
  try {
    if (t.graph() && !t.graph()->nodes().at(static_cast<size_t>(t.id())).name.empty()) {
      out << t.graph()->nodes().at(static_cast<size_t>(t.id())).name << " (" << t.node_type()
          << ", " << t.value() << ")";
    } else {
      out << "node_" << t.id() << " (" << t.node_type() << ", " << t.value() << ")";
    }
    return out;
  } catch (const std::exception& e) {
    RT_PANIC("Invalid index into nodes array of graph: " << e.what());
  }
}

// - Register record type --------------------------------------------------------------------------
template <typename T>
requires is_record_type_v<T>
constexpr void register_variable(const T& rt,
                                 std::shared_ptr<Graph<decay_record_type_t<T>>> graph) noexcept {
  rt.register_graph(graph);
}

template <FwdContainerType CT, typename PassiveType>
constexpr void register_variable(const CT& container,
                                 std::shared_ptr<Graph<PassiveType>> graph) noexcept {
  std::for_each(std::cbegin(container), std::cend(container), [&](const auto& rt) {
    rt.register_graph(graph);
  });
}

// - Mark variable as output -----------------------------------------------------------------------
template <typename T>
requires is_record_type_v<T>
constexpr void mark_output(const T& rt) noexcept {
  rt.mark_output();
}

template <FwdContainerType CT>
constexpr void mark_output(const CT& container) noexcept {
  std::for_each(
      std::cbegin(container), std::cend(container), [](const auto& rt) { rt.mark_output(); });
}

// - Add name to variable --------------------------------------------------------------------------
template <typename T>
requires is_record_type_v<T>
constexpr void add_name(const T& rt, const std::string& name) noexcept {
  rt.add_name(name);
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
