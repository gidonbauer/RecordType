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
  // TODO: Think about possible options
  void to_dot(const std::string& file_name) const {
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

#endif  // RT_GRAPH_HPP_
