#ifndef RT_TO_PYTHON_HPP_
#define RT_TO_PYTHON_HPP_

#include <fstream>
#include <string>
#include <vector>

#include "Graph.hpp"
#include "Macros.hpp"

namespace RT {

// TODO: Test `to_python`
template <typename PassiveType>
void to_python(const Graph<PassiveType>* graph, const std::string& filename) {
  // - Setup -------------------------------------------------------------------
  using namespace std::string_literals;
  constexpr auto single_indent = "    ";
  const auto var_prefix        = "v"s;
  const auto make_var          = [&](id_t id) { return var_prefix + std::to_string(id); };
  // - Setup -------------------------------------------------------------------

  // - Find input and output variables -----------------------------------------
  std::vector<id_t> input_variables{};
  std::vector<PassiveType> input_values{};
  std::vector<id_t> output_variables{};
  std::vector<PassiveType> output_values{};
  for (id_t id_count = 0; const auto& node : graph->nodes()) {
    if (node.io_type == IOType::INPUT) {
      input_variables.push_back(id_count);
      input_values.push_back(node.value);
    } else if (node.io_type == IOType::OUTPUT) {
      output_variables.push_back(id_count);
      output_values.push_back(node.value);
    }
    ++id_count;
  }
  // - Find input and output variables -----------------------------------------

  std::ofstream out(filename);
  if (!out) {
    throw std::runtime_error(RT_ERROR_LOC() + ": Could not open file `"s + filename + "`: "s +
                             std::strerror(errno));
  }

  out << "import math\n\n\n";

  out << "def f(";
  for (auto id : input_variables) {
    out << make_var(id) << ',';
  }
  out << "):\n";

  for (id_t id_count = 0; const auto& node : graph->nodes()) {
    const auto num_deps = node.dependencies.size();
    if (node.io_type == IOType::INPUT) {
      ++id_count;
      continue;
    }

    switch (node.type) {
      case NodeType::LITERAL:
        RT_TODO("Node type `" << node.type << "` is not implemented yet.");
        break;
      case NodeType::VAR:
        {
          RT_ASSERT(num_deps == 0UL || num_deps == 1UL,
                    "Expected node to have zero or one dependencies, but got " << num_deps);
          if (num_deps == 0UL) {
            out << single_indent << make_var(id_count) << " = " << node.value << '\n';
          } else {
            out << single_indent << make_var(id_count) << " = " << make_var(node.dependencies[0])
                << '\n';
          }
        }
        break;
      case NodeType::ADD:
        {
          RT_ASSERT(num_deps == 2UL,
                    "Expected node to have two dependencies, but got " << num_deps);
          out << single_indent << make_var(id_count) << " = " << make_var(node.dependencies[0])
              << " + " << make_var(node.dependencies[1]) << '\n';
        }
        break;
      case NodeType::SUB:
        {
          RT_ASSERT(num_deps == 2UL,
                    "Expected node to have two dependencies, but got " << num_deps);
          out << single_indent << make_var(id_count) << " = " << make_var(node.dependencies[0])
              << " - " << make_var(node.dependencies[1]) << '\n';
        }
        break;
      case NodeType::MUL:
        {
          RT_ASSERT(num_deps == 2UL,
                    "Expected node to have two dependencies, but got " << num_deps);
          out << single_indent << make_var(id_count) << " = " << make_var(node.dependencies[0])
              << " * " << make_var(node.dependencies[1]) << '\n';
        }
        break;
      case NodeType::DIV:
        {
          RT_ASSERT(num_deps == 2UL,
                    "Expected node to have two dependencies, but got " << num_deps);
          out << single_indent << make_var(id_count) << " = " << make_var(node.dependencies[0])
              << " / " << make_var(node.dependencies[1]) << '\n';
        }
        break;
      case NodeType::INV:
        {
          RT_ASSERT(num_deps == 1UL, "Expected node to have one dependency, but got " << num_deps);
          out << single_indent << make_var(id_count) << " = 1/" << make_var(node.dependencies[0])
              << '\n';
        }
        break;
      case NodeType::NEG:
        {
          RT_ASSERT(num_deps == 1UL, "Expected node to have one dependency, but got " << num_deps);
          out << single_indent << make_var(id_count) << " = -" << make_var(node.dependencies[0])
              << '\n';
        }
        break;
      case NodeType::SQRT:
        {
          RT_ASSERT(num_deps == 1UL, "Expected node to have one dependency, but got " << num_deps);
          out << single_indent << make_var(id_count) << " = math.sqrt("
              << make_var(node.dependencies[0]) << ")\n";
        }
        break;
      case NodeType::SIN:
        {
          RT_ASSERT(num_deps == 1UL, "Expected node to have one dependency, but got " << num_deps);
          out << single_indent << make_var(id_count) << " = math.sin("
              << make_var(node.dependencies[0]) << ")\n";
        }
        break;
      case NodeType::COS:
        {
          RT_ASSERT(num_deps == 1UL, "Expected node to have one dependency, but got " << num_deps);
          out << single_indent << make_var(id_count) << " = math.cos("
              << make_var(node.dependencies[0]) << ")\n";
        }
        break;
      default:
        RT_PANIC("Unknown node type `" << node.type << "`.");
    }
    ++id_count;
  }

  out << single_indent << "return [";
  for (id_t id : output_variables) {
    out << make_var(id) << ", ";
  }
  out << "]\n\n\n";

  out << "def main():\n";
  out << single_indent << "x = [";
  for (const auto& val : input_values) {
    out << val << ", ";
  }
  out << "]\n";

  out << single_indent << "y = f(*x)\n";

  out << single_indent << "y_exp = [";
  for (const auto& val : output_values) {
    out << val << ", ";
  }
  out << "]\n";

  out << single_indent << "print(f\"f({x}) = {y}\")\n";
  out << single_indent << "print(f\"expected: {y_exp}\")\n";
  out << "\n\n";

  out << "if __name__ == \"__main__\":\n";
  out << single_indent << "main()\n";
}

}  // namespace RT

#endif  // RT_TO_PYTHON_HPP_
