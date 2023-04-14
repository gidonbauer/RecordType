#ifndef RT_NODE_TYPE_HPP_
#define RT_NODE_TYPE_HPP_

#include <iosfwd>

#include "Macros.hpp"

namespace RT {

enum class NodeType {
  LITERAL,
  VAR,
  ADD,
  MUL,
  INV,
  NEG,
  SQRT,
  NODE_TYPE_COUNT,
};

[[nodiscard]] constexpr auto is_op(NodeType node_type) noexcept -> bool {
  // TODO: Are NodeType::INV and NodeType::NEG operations that we want to count?
  static_assert(static_cast<int>(NodeType::NODE_TYPE_COUNT) == 7,
                "Number of node types changed, are the new ones operations?");
  return node_type == NodeType::ADD || node_type == NodeType::MUL || node_type == NodeType::SQRT;
}

auto operator<<(std::ostream& out, NodeType node_type) noexcept -> std::ostream& {
  static_assert(static_cast<int>(NodeType::NODE_TYPE_COUNT) == 7,
                "Number of node types changed, add name to switch statement.");
  switch (node_type) {
    case NodeType::LITERAL:
      out << "LITERAL";
      break;
    case NodeType::VAR:
      out << "VAR";
      break;
    case NodeType::ADD:
      out << "ADD";
      break;
    case NodeType::MUL:
      out << "MUL";
      break;
    case NodeType::INV:
      out << "INV";
      break;
    case NodeType::NEG:
      out << "NEG";
      break;
    case NodeType::SQRT:
      out << "SQRT";
      break;
    default:
      RT_PANIC("Unknown NodeType: `" << static_cast<int>(node_type) << "`.");
  }

  return out;
}

}  // namespace RT

#endif  // RT_NODE_TYPE_HPP_
