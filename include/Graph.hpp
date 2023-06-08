#ifndef RT_GRAPH_HPP_
#define RT_GRAPH_HPP_

#include <fstream>
#include <numeric>
#include <vector>

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

// TODO: Is the graph thread safe? We only add nodes and edges and never remove any.
template <typename PassiveType>
class Graph {
  std::vector<int64_t> m_dependencies{};
  std::vector<NodeType> m_operations{};
  std::vector<PassiveType> m_values{};

 public:
  constexpr Graph() noexcept                                = default;
  constexpr Graph(const Graph&) noexcept                    = delete;
  constexpr Graph(Graph&&) noexcept                         = delete;
  constexpr auto operator=(const Graph&) noexcept -> Graph& = delete;
  constexpr auto operator=(Graph&&) noexcept -> Graph&      = delete;
  constexpr ~Graph() noexcept                               = default;

  // -----------------------------------------------------------------------------------------------
  template <typename... IDS>
  requires(std::is_same_v<IDS, uint64_t>, ...)
  constexpr void add_dependencies(IDS&&... ids) noexcept {
    if constexpr (sizeof...(ids) == 0) {
      return;
    }

    constexpr uint64_t sign_bit = 1ull << 63ull;
    assert((... & !(ids & sign_bit)) && "All ids must be less than UINT64_MAX");

    (m_dependencies.push_back(static_cast<int64_t>(ids)), ...);
    m_dependencies.push_back(-static_cast<int64_t>(sizeof...(ids)));
  };

  // -----------------------------------------------------------------------------------------------
  constexpr void add_operation(uint64_t id, NodeType op, PassiveType value) noexcept {
    constexpr uint64_t sign_bit = 1ull << 63ull;
    assert(!(id & sign_bit) && "id must be less than UINT64_MAX");

    assert(m_operations.size() == id);
    m_dependencies.push_back(static_cast<int64_t>(id));
    m_operations.push_back(op);
    m_values.push_back(std::move(value));
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto count_ops() const noexcept -> size_t {
    return std::accumulate(
        std::cbegin(m_operations),
        std::cend(m_operations),
        0ul,
        [](size_t count, NodeType node_type) { return count + is_op(node_type); });
  }

  // -----------------------------------------------------------------------------------------------
  [[nodiscard]] constexpr auto count_op(NodeType op) const noexcept -> size_t {
    return std::accumulate(
        std::cbegin(m_operations),
        std::cend(m_operations),
        0ul,
        [op](size_t count, NodeType node_type) { return count + (node_type == op); });
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

    assert(m_operations.size() == m_values.size());
    for (size_t id = 0ul; id < m_operations.size(); ++id) {
      out << "  node_" << id << " [label=\"node_" << id << " (" << m_operations[id] << ", "
          << m_values[id] << ")\"];\n";
    }

    for (auto it = std::crbegin(m_dependencies); it != std::crend(m_dependencies);) {
      auto to_id = *(it++);
      assert(to_id >= 0);

      if (it == std::crend(m_dependencies)) {
        break;
      }

      auto num_deps = *(it++);
      if (num_deps >= 0) {
        --it;
        continue;
      }

      for (int64_t i = 0; i < -num_deps; ++i) {
        auto from_id = *(it++);
        assert(from_id >= 0);
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

#endif  // RT_GRAPH_HPP_
