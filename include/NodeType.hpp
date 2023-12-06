#ifndef RT_NODE_TYPE_HPP_
#define RT_NODE_TYPE_HPP_

#include <iosfwd>

#include "Macros.hpp"

namespace RT {

// - Node type -------------------------------------------------------------------------------------
enum class NodeType {
  LITERAL,
  VAR,
  ADD,
  SUB,
  MUL,
  DIV,
  INV,
  NEG,
  SQRT,
  SIN,
  COS,
  NODE_TYPE_COUNT,
};

// - Node type to string ---------------------------------------------------------------------------
constexpr auto to_string(NodeType node_type) noexcept -> std::string {
  static_assert(static_cast<int>(NodeType::NODE_TYPE_COUNT) == 11,
                "Number of node types changed, add name to switch statement.");
  using namespace std::string_literals;

  switch (node_type) {
    case NodeType::LITERAL:
      return "LITERAL"s;
    case NodeType::VAR:
      return "VAR"s;
    case NodeType::ADD:
      return "ADD"s;
    case NodeType::SUB:
      return "SUB"s;
    case NodeType::MUL:
      return "MUL"s;
    case NodeType::DIV:
      return "DIV"s;
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

// - Write node type to ostream --------------------------------------------------------------------
auto operator<<(std::ostream& out, NodeType node_type) noexcept -> std::ostream& {
  out << to_string(node_type);
  return out;
}

// - Input or output type --------------------------------------------------------------------------
enum class IOType {
  INTERMEDIATE,
  INPUT,
  OUTPUT,
  IO_TYPE_COUNT,
};

// - IO-type to string -----------------------------------------------------------------------------
constexpr auto to_string(IOType io_type) noexcept -> std::string {
  static_assert(static_cast<int>(IOType::IO_TYPE_COUNT) == 3,
                "Number of io-types changed, add name to switch statement.");
  using namespace std::string_literals;

  switch (io_type) {
    case IOType::INPUT:
      return "INPUT"s;
    case IOType::OUTPUT:
      return "OUTPUT"s;
    case IOType::INTERMEDIATE:
      return "INTERMEDIATE"s;
    default:
      RT_PANIC("Unknown IOType: `" << static_cast<int>(io_type) << "`.");
  }
}

// - Write io-type to ostream --------------------------------------------------------------------..
auto operator<<(std::ostream& out, IOType io_type) noexcept -> std::ostream& {
  out << to_string(io_type);
  return out;
}
}  // namespace RT

#endif  // RT_NODE_TYPE_HPP_
