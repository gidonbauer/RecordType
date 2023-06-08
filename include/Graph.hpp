#ifndef RT_GRAPH_HPP_
#define RT_GRAPH_HPP_

// TODO: Is the graph thread safe? We only add nodes and edges and never remove any.

#include <algorithm>
#include <cstring>
#include <fstream>
#include <numeric>
#include <optional>
#include <vector>

#include "Macros.hpp"
#include "NodeType.hpp"
#include "RecordType.hpp"

namespace RT {

template <typename PassiveType>
class RecordType;

struct GraphToDotOptions {
  bool unique_literals      = true;   // Number literals are unique nodes
  bool number_only_literals = false;  // Number literals are represented by only their value
  bool print_node_id        = false;  // Print the node id in the graph
  bool use_op_symbols       = false;  // Operatrion like ADD and MUL are shown as Symbols
};

template <typename PassiveType>
class Graph {
 public:
  struct Node {
    PassiveType value;
    uint64_t id;
    NodeType node_type;
    std::vector<size_t> adjacent_nodes{};

    Node()
        : Node(static_cast<PassiveType>(0), static_cast<uint64_t>(-1), NodeType::LITERAL) {}
    Node(PassiveType value_, uint64_t id_, NodeType node_type_)
        : value(std::move(value_)),
          id(id_),
          node_type(node_type_) {}
  };

  enum : int {
    INSERT_FROM_NODE = 0b001,
    INSERT_TO_NODE   = 0b010,
    INSERT_EDGE      = 0b100,
  };

 private:
  std::vector<Node> m_nodes{};

 public:
  constexpr auto insert_node_edge(const RecordType<PassiveType>& to_insert_from,
                                  const RecordType<PassiveType>& to_insert_to) noexcept -> int {
    int res = 0;
    if (insert_node(to_insert_from)) {
      res |= INSERT_FROM_NODE;
    }
    if (insert_node(to_insert_to)) {
      res |= INSERT_TO_NODE;
    }
    if (insert_edge(to_insert_from.id(), to_insert_to.id())) {
      res |= INSERT_EDGE;
    }
    return res;
  }

  constexpr auto insert_node(const RecordType<PassiveType>& to_insert) noexcept -> bool {
    const auto opt_idx = find_index(to_insert.id());
    if (opt_idx.has_value()) {
      return false;
    }
    m_nodes.emplace_back(to_insert.value(), to_insert.id(), to_insert.node_type());
    return true;
  }

  constexpr auto insert_edge(uint64_t from_id, uint64_t to_id) noexcept -> bool {
    const auto opt_from_idx = find_index(from_id);
    const auto opt_to_idx   = find_index(to_id);
    if (!opt_from_idx.has_value() || !opt_to_idx.has_value()) {
      return false;
    }

    Node& from_node = m_nodes[*opt_from_idx];
    if (std::find(std::cbegin(from_node.adjacent_nodes),
                  std::cend(from_node.adjacent_nodes),
                  *opt_to_idx) != std::cend(from_node.adjacent_nodes)) {
      return false;
    }

    from_node.adjacent_nodes.push_back(*opt_to_idx);
    return true;
  }

  [[nodiscard]] constexpr auto find_index(uint64_t id) const noexcept -> std::optional<size_t> {
    for (size_t i = 0ul; i < m_nodes.size(); ++i) {
      if (m_nodes[i].id == id) {
        return i;
      }
    }
    return std::nullopt;
  }

  [[nodiscard]] constexpr auto get(uint64_t id) const -> const Node& {
    const auto opt_idx = find_index(id);
    if (!opt_idx.has_value()) {
      throw std::runtime_error("Node with id " + std::to_string(id) + " is not in graph.");
    }
    return m_nodes[*opt_idx];
  }

  [[nodiscard]] constexpr auto count_ops() const noexcept -> size_t {
    return std::accumulate(
        std::cbegin(m_nodes), std::cend(m_nodes), 0ul, [](size_t count, const Node& node) {
          return count + is_op(node.node_type);
        });
  }

  [[nodiscard]] constexpr auto count_op(NodeType op) const noexcept -> size_t {
    return std::accumulate(
        std::cbegin(m_nodes), std::cend(m_nodes), 0ul, [op](size_t count, const Node& node) {
          return count + (node.node_type == op);
        });
  }

  void to_dot(const std::string& file_name, const GraphToDotOptions& opt = {}) const {
    std::ofstream out(file_name);
    if (!out) {
      throw std::runtime_error("Could not open file `" + file_name + "`: " + std::strerror(errno));
    }

    // Begin Graph
    out << "digraph {\n";

    // Print all nodes and how they should be represented
    // NOLINTBEGIN(cppcoreguidelines-avoid-goto)
    for (const auto& node : m_nodes) {
      static_assert(
          static_cast<int>(NodeType::NODE_TYPE_COUNT) == 7,
          "Number of node types changed, it might be necessary to adjust the graph output.");
      switch (node.node_type) {
        case NodeType::LITERAL:
          if (opt.number_only_literals) {
            if (opt.unique_literals) {
              out << "    node_" << node.id << " [label=\"" << node.value << "\"];\n";
              break;
            }
          }
          if (!opt.unique_literals) {
            out << "    literal_" << node.value << " [label=\"" << node.value << "\"];\n";
            break;
          }
          goto default_case;

        case NodeType::MUL:
          if (opt.use_op_symbols) {
            out << "    node_" << node.id << " [label=\"*\"];\n";
            break;
          }
          goto default_case;

        case NodeType::ADD:
          if (opt.use_op_symbols) {
            out << "    node_" << node.id << " [label=\"+\"];\n";
            break;
          }
          goto default_case;

        default_case:
        default:
          if (opt.print_node_id) {
            out << "    node_" << node.id << " [label=\"node_" << node.id << " (" << node.node_type
                << ", " << node.value << ")\"];\n";
          } else {
            out << "    node_" << node.id << " [label=\"(" << node.node_type << ", " << node.value
                << ")\"];\n";
          }
      }
    }
    out << '\n';
    // NOLINTEND(cppcoreguidelines-avoid-goto)

    // Print all edges
    for (const auto& node : m_nodes) {
      for (const auto& adj_node_idx : node.adjacent_nodes) {
        if (!opt.unique_literals) {
          if (node.node_type == NodeType::LITERAL) {
            out << "    literal_" << node.value;
          } else {
            out << "    node_" << node.id;
          }
          out << " -> ";
          if (m_nodes[adj_node_idx].node_type == NodeType::LITERAL) {
            out << "literal_" << m_nodes[adj_node_idx].value;
          } else {
            out << "node_" << m_nodes[adj_node_idx].id;
          }
          out << ";\n";
        } else {
          out << "    node_" << node.id << " -> node_" << m_nodes[adj_node_idx].id << ";\n";
        }
      }
    }

    // End graph
    out << "}\n";
  }
};

}  // namespace RT

#endif  // RT_GRAPH_HPP_
