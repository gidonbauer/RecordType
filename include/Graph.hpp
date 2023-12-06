#ifndef RT_GRAPH_HPP_
#define RT_GRAPH_HPP_

#include <cstring>
#include <format>
#include <fstream>
#include <numeric>
#include <vector>

#include "Definitions.hpp"
#include "NodeType.hpp"

namespace RT {

template <typename PassiveType>
class RecordType;

struct GraphToDotOptions {
  bool unique_literals      = true;   // Number literals are unique nodes
  bool number_only_literals = false;  // Number literals are represented by only their value
  bool print_node_id        = false;  // Print the node id in the graph
  bool use_op_symbols       = false;  // Operatrion like ADD and MUL are shown as Symbols
};

namespace internal {

template <typename PassiveType>
struct Node {
  NodeType type{};
  IOType io_type{};
  PassiveType value{};
  std::vector<id_t> dependencies{};
};

}  // namespace internal

template <typename PassiveType>
class Graph {
  using Node = internal::Node<PassiveType>;

  std::vector<Node> m_nodes{};

 public:
  // - Get nodes -----------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto nodes() const noexcept -> const std::vector<Node>& {
    return m_nodes;
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto add_operation(NodeType type,
                                             PassiveType value,
                                             std::vector<id_t> dependencies = {}) noexcept -> id_t {
    for (auto idx : dependencies) {
      RT_ASSERT(m_nodes.size() > 0UL, "Graph is empty, cannot have any dependencies.");
      RT_ASSERT(idx >= static_cast<id_t>(0), "At least one dependency is not registerd.");
      RT_ASSERT(static_cast<size_t>(idx) < m_nodes.size(),
                "Dependency with index " << idx << " is not in graph; max. index is "
                                         << m_nodes.size() - 1UL);
    }
    const auto new_idx = static_cast<id_t>(m_nodes.size());
    m_nodes.emplace_back(type, IOType::INTERMEDIATE, std::move(value), std::move(dependencies));
    return new_idx;
  }

  // -----------------------------------------------------------------------------------------------
  constexpr void mark_input(id_t id) noexcept {
    RT_ASSERT(m_nodes.size() > 0UL, "Graph is empty, cannot register any variable as input.");
    RT_ASSERT(id >= static_cast<id_t>(0), "Variable is not registerd in graph.");
    RT_ASSERT(static_cast<size_t>(id) < m_nodes.size(),
              "id " << id << " is not in graph, max. id is " << m_nodes.size() - 1UL);
    m_nodes[static_cast<size_t>(id)].io_type = IOType::INPUT;
  }

  // -----------------------------------------------------------------------------------------------
  constexpr void mark_output(id_t id) noexcept {
    RT_ASSERT(m_nodes.size() > 0UL, "Graph is empty, cannot register any variable as output.");
    RT_ASSERT(id >= static_cast<id_t>(0), "Variable is not registerd in graph.");
    RT_ASSERT(static_cast<size_t>(id) < m_nodes.size(),
              "id " << id << " is not in graph, max. id is " << m_nodes.size() - 1UL);
    m_nodes[static_cast<size_t>(id)].io_type = IOType::OUTPUT;
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto count_op(NodeType op) const noexcept -> size_t {
    return std::accumulate(
        std::cbegin(m_nodes), std::cend(m_nodes), 0ul, [op](size_t count, const Node& node) {
          return count + static_cast<size_t>(node.type == op);
        });
  }

  [[nodiscard]] constexpr auto count_ops(const std::vector<NodeType>& ops) const noexcept
      -> size_t {
    return std::accumulate(std::cbegin(ops),
                           std::cend(ops),
                           0ul,
                           [this](size_t count, NodeType op) { return count + count_op(op); });
  }

  // -----------------------------------------------------------------------------------------------
  void to_dot(const std::string& file_name,
              [[maybe_unused]] const GraphToDotOptions& opt = {}) const {
    std::ofstream out(file_name);
    if (!out) {
      throw std::runtime_error("Could not open file `" + file_name + "`: " + std::strerror(errno));
    }

    // Begin Graph
    out << "digraph {\n";
    id_t id_count = 0;
    for (const auto& node : m_nodes) {
      out << std::format("  node_{0} [label=\"node_{0} ({1}, {2})\"];\n",
                         id_count,
                         to_string(node.type),
                         node.value);
      int dep_count = 0;
      for (id_t dep_id : node.dependencies) {
        out << std::format(
            "  node_{0} -> node_{1} [label=\"{2}\"];\n", dep_id, id_count, dep_count);
        ++dep_count;
      }
      ++id_count;
    }

    // End graph
    out << "}\n";
  }

  // -----------------------------------------------------------------------------------------------
  void dump_data(std::ostream& out) const {
    id_t id_count = 0;
    for (const auto& node : m_nodes) {
      out << "{\n";
      out << "  id = " << id_count++ << '\n';
      out << "  type = " << node.type << '\n';
      out << "  io_type = " << node.io_type << '\n';
      out << "  value = " << node.value << '\n';
      out << "  dependencies = [ ";
      for (id_t dep : node.dependencies) {
        out << dep << ", ";
      }
      out << "]\n";
      out << "}\n";
    }
  }
};

}  // namespace RT

#if 0

namespace RT {

template <typename PassiveType>
class RecordType;

struct GraphToDotOptions {
  bool unique_literals      = true;   // Number literals are unique nodes
  bool number_only_literals = false;  // Number literals are represented by only their value
  bool print_node_id        = false;  // Print the node id in the graph
  bool use_op_symbols       = false;  // Operatrion like ADD and MUL are shown as Symbols
};

template <typename T>
concept RecordTypeId = std::is_same_v<std::remove_cv_t<std::remove_reference_t<T>>, int64_t>;

// TODO: Is the graph thread safe? We only add nodes and edges and never remove any.
template <typename PassiveType>
class Graph {
  std::vector<int64_t> m_dependencies{};
  std::vector<NodeType> m_operations{};
  std::vector<PassiveType> m_values{};

 public:
  // -----------------------------------------------------------------------------------------------
  template <RecordTypeId... IDS>
  constexpr void add_dependencies(IDS&&... ids) noexcept {
    if constexpr (sizeof...(ids) == 0) {
      return;
    }
    (m_dependencies.push_back(ids), ...);
    m_dependencies.push_back(-static_cast<int64_t>(sizeof...(ids)));
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto add_operation(NodeType op, PassiveType value) noexcept -> int64_t {
    RT_ASSERT(m_operations.size() == m_values.size(),
              "`m_operations` and `m_values` must have same size, but sizes are size(m_operations)="
                  << m_operations.size() << " and size(m_values)=" << m_values.size());

    // TODO: Add a check here for the correct number of dependencies; make disableable
#ifdef RT_GRAPH_CHECK_ADD_OP
    static_assert(static_cast<int>(NodeType::NODE_TYPE_COUNT) == 9,
                  "This code assumes that the number of node types is 9.");
    if (!m_dependencies.empty()) {
      const auto num_deps = m_dependencies.back();
      if (num_deps < 0) {
        switch (op) {
          case NodeType::LITERAL:
            RT_ASSERT(num_deps == -1, "LITERAL expects one dependency, got " << -num_deps);
            break;
          case NodeType::VAR:
            RT_ASSERT(num_deps == -1, "VAR expects one dependency, got " << -num_deps);
            break;
          case NodeType::ADD:
            RT_ASSERT(num_deps == -2, "ADD expects two dependencies, got " << -num_deps);
            break;
          case NodeType::MUL:
            RT_ASSERT(num_deps == -2, "MUL expects two dependencies, got " << -num_deps);
            break;
          case NodeType::INV:
            RT_ASSERT(num_deps == -1, "INV expects one dependency, got " << -num_deps);
            break;
          case NodeType::NEG:
            RT_ASSERT(num_deps == -1, "NEG expects one dependency, got " << -num_deps);
            break;
          case NodeType::SQRT:
            RT_ASSERT(num_deps == -1, "SQRT expects one dependency, got " << -num_deps);
            break;
          case NodeType::SIN:
            RT_ASSERT(num_deps == -1, "SIN expects one dependency, got " << -num_deps);
            break;
          case NodeType::COS:
            RT_ASSERT(num_deps == -1, "COS expects one dependency, got " << -num_deps);
            break;
          default:
            RT_PANIC("Invalid node type " << static_cast<int>(op));
        }
      }
    }
#endif  // RT_GRAPH_CHECK_ADD_OP

    const auto id = static_cast<int64_t>(m_operations.size());
    m_dependencies.push_back(id);
    m_operations.push_back(op);
    m_values.push_back(std::move(value));
    return id;
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto count_ops() const noexcept -> size_t {
    return std::accumulate(std::cbegin(m_operations),
                           std::cend(m_operations),
                           0ul,
                           [](size_t count, NodeType node_type) {
                             return count + static_cast<size_t>(is_op(node_type));
                           });
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto count_op(NodeType op) const noexcept -> size_t {
    return std::accumulate(std::cbegin(m_operations),
                           std::cend(m_operations),
                           0ul,
                           [op](size_t count, NodeType node_type) {
                             return count + static_cast<size_t>(node_type == op);
                           });
  }

  // -----------------------------------------------------------------------------------------------
  // TODO: Use GraphToDotOptions
  void to_dot(const std::string& file_name,
              [[maybe_unused]] const GraphToDotOptions& opt = {}) const {
    std::ofstream out(file_name);
    if (!out) {
      throw std::runtime_error("Could not open file `" + file_name + "`: " + std::strerror(errno));
    }

    // Begin Graph
    out << "digraph {\n";

    RT_ASSERT(m_operations.size() == m_values.size(),
              "`m_operations` and `m_values` must have same size, but sizes are size(m_operations)="
                  << m_operations.size() << " and size(m_values)=" << m_values.size());
    for (size_t id = 0ul; id < m_operations.size(); ++id) {
      out << "  node_" << id << " [label=\"node_" << id << " (" << m_operations[id] << ", "
          << m_values[id] << ")\"];\n";
    }

    for (auto it = std::crbegin(m_dependencies); it != std::crend(m_dependencies);) {
      const auto to_id = *(it++);
      RT_ASSERT(to_id >= 0,
                "`to_id` must be greater or equal to 0, otherwise it would be the number of "
                "dependencies. Is "
                    << to_id);

      if (it == std::crend(m_dependencies)) {
        break;
      }

      const auto num_deps = *(it++);
      if (num_deps >= 0) {
        --it;
        continue;
      }

      for (int64_t i = 0; i < -num_deps; ++i) {
        RT_ASSERT(it != std::crend(m_dependencies),
                  "`it` should not have reached the end of the array");
        const auto from_id = *(it++);
        RT_ASSERT(from_id >= 0,
                  "`from_id` must be greater or equal to 0, otherwise it would be the number of "
                  "dependencies. Is "
                      << from_id);

        out << "  node_" << from_id << " -> node_" << to_id << ";\n";
      }
    }

    // End graph
    out << "}\n";
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto dependencies() const noexcept -> const std::vector<int64_t>& {
    return m_dependencies;
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto operations() const noexcept -> const std::vector<NodeType>& {
    return m_operations;
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto values() const noexcept -> const std::vector<PassiveType>& {
    return m_values;
  }

  // -----------------------------------------------------------------------------------------------
  constexpr void dump_data(std::ostream& out) const noexcept {
    out << "dependencies: ";
    for (auto dep : m_dependencies) {
      out << dep << ' ';
    }
    out << '\n';

    out << "operations: ";
    for (auto op : m_operations) {
      out << op << ' ';
    }
    out << '\n';

    out << "values: ";
    for (const auto& val : m_values) {
      out << val << ' ';
    }
    out << '\n';
  }
};

}  // namespace RT
#endif

#endif  // RT_GRAPH_HPP_
