#ifndef RT_NODE_TYPE_HPP_
#define RT_NODE_TYPE_HPP_

#include <iosfwd>

#include "Macros.hpp"

namespace RT {

// -------------------------------------------------------------------------------------------------
enum class NodeType {
  LITERAL,
  VAR,
  ADD,
  MUL,
  INV,
  NEG,
  SQRT,
  SIN,
  COS,
  NODE_TYPE_COUNT,
};

// -------------------------------------------------------------------------------------------------
[[nodiscard]] constexpr auto is_op(NodeType node_type) noexcept -> bool {
  // TODO: Are NodeType::INV and NodeType::NEG operations that we want to count?
  static_assert(static_cast<int>(NodeType::NODE_TYPE_COUNT) == 9,
                "Number of node types changed, are the new ones operations?");
  return node_type == NodeType::ADD || node_type == NodeType::MUL || node_type == NodeType::SQRT ||
         node_type == NodeType::SIN || node_type == NodeType::COS;
}

// -------------------------------------------------------------------------------------------------
constexpr auto to_string(NodeType node_type) noexcept -> std::string {
  static_assert(static_cast<int>(NodeType::NODE_TYPE_COUNT) == 9,
                "Number of node types changed, add name to switch statement.");
  using namespace std::string_literals;

  switch (node_type) {
    case NodeType::LITERAL:
      return "LITERAL"s;
    case NodeType::VAR:
      return "VAR"s;
    case NodeType::ADD:
      return "ADD"s;
    case NodeType::MUL:
      return "MUL"s;
    case NodeType::INV:
      return "INV"s;
    case NodeType::NEG:
      return "NEG"s;
    case NodeType::SQRT:
      return "SQRT"s;
    case NodeType::SIN:
      return "SIN"s;
    case NodeType::COS:
      return "COS"s;
    default:
      RT_PANIC("Unknown NodeType: `" << static_cast<int>(node_type) << "`.");
  }
}

// -------------------------------------------------------------------------------------------------
auto operator<<(std::ostream& out, NodeType node_type) noexcept -> std::ostream& {
  out << to_string(node_type);
  return out;
}

}  // namespace RT

#endif  // RT_NODE_TYPE_HPP_
