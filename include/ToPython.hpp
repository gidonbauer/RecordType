#ifndef RT_TO_PYTHON_HPP_
#define RT_TO_PYTHON_HPP_

#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "Graph.hpp"
#include "IteratorReverser.hpp"
#include "Macros.hpp"

namespace RT {

// TODO: 1) Handle output variable properly, a variable might be an output variable but still be
//          used in a calculation
//       2) Test `to_python`
template <typename PassiveType>
void to_python(const Graph<PassiveType>* graph, const std::string& filename) {
  // - Setup -------------------------------------------------------------------
  using namespace std::string_literals;
  constexpr auto single_indent = "    ";
  const auto var_prefix        = "v"s;
  const auto make_var          = [&](int64_t id) { return var_prefix + std::to_string(id); };
  // - Setup -------------------------------------------------------------------

  // - Generate expressions ----------------------------------------------------
  const auto& deps = graph->dependencies();
  const auto& ops  = graph->operations();
  const auto& vals = graph->values();

  std::vector<int64_t> input_variables{};
  std::vector<PassiveType> input_values{};
  std::vector<std::string> expressions{};
  std::vector<int64_t> possible_output_variables{};
  std::unordered_set<int64_t> used_variables{};

  auto op_it   = std::crbegin(ops);
  auto vals_it = std::crbegin(vals);
  auto dep_it  = std::crbegin(deps);
  while (dep_it != std::crend(deps)) {
    const auto to_id = *(dep_it++);
    RT_ASSERT(to_id >= 0,
              "`to_id` must be greater or equal to 0, otherwise it would be the number of "
              "dependencies. Is "
                  << to_id);

    if (dep_it == std::crend(deps)) {
      input_variables.push_back(to_id);
      RT_ASSERT(vals_it != std::crend(vals),
                "`vals_it` should not have reached the end of the array");
      input_values.push_back(*vals_it++);
      break;
    }

    const auto num_deps = *(dep_it++);
    if (num_deps >= 0) {
      --dep_it;
      input_variables.push_back(to_id);
      RT_ASSERT(vals_it != std::crend(vals),
                "`vals_it` should not have reached the end of the array");
      input_values.push_back(*vals_it++);
      continue;
    }

    RT_ASSERT(vals_it != std::crend(vals),
              "`vals_it` should not have reached the end of the array");
    ++vals_it;
    possible_output_variables.push_back(to_id);

    std::string expr = make_var(to_id) + " = "s;

    RT_ASSERT(op_it != std::crend(ops), "`op_it` should not have reached the end of the array");
    const auto op = *op_it++;

    switch (op) {
      case NodeType::VAR:
        {
          RT_ASSERT(num_deps == -1, "Expected one dependency, but got " << -num_deps);
          const auto dep = *(dep_it++);
          used_variables.insert(dep);
          expr += make_var(dep);
        }
        break;

      case NodeType::ADD:
        {
          RT_ASSERT(num_deps == -2, "Expected two dependencies, but got" << -num_deps);
          const auto dep1 = *(dep_it++);
          used_variables.insert(dep1);
          const auto dep2 = *(dep_it++);
          used_variables.insert(dep2);
          expr += make_var(dep1) + " + " + make_var(dep2);
        }
        break;

      case NodeType::MUL:
        {
          RT_ASSERT(num_deps == -2, "Expected two dependencies, but got " << -num_deps);
          const auto dep1 = *(dep_it++);
          used_variables.insert(dep1);
          const auto dep2 = *(dep_it++);
          used_variables.insert(dep2);
          expr += make_var(dep1) + " * " + make_var(dep2);
        }
        break;

      case NodeType::INV:
        {
          RT_ASSERT(num_deps == -1, "Expected one dependency, but got " << -num_deps);
          const auto dep = *(dep_it++);
          used_variables.insert(dep);
          expr += "1 / "s + make_var(dep);
        }
        break;

      case NodeType::NEG:
        {
          RT_ASSERT(num_deps == -1, "Expected one dependency, but got " << -num_deps);
          const auto dep = *(dep_it++);
          used_variables.insert(dep);
          expr += "-"s + make_var(dep);
        }
        break;

      case NodeType::SQRT:
        {
          RT_ASSERT(num_deps == -1, "Expected one dependency, but got" << -num_deps);
          const auto dep = *(dep_it++);
          used_variables.insert(dep);
          expr += "math.sqrt("s + make_var(dep) + ")"s;
        }
        break;

      case NodeType::SIN:
        {
          RT_ASSERT(num_deps == -1, "Expected one dependency, but got " << -num_deps);
          const auto dep = *(dep_it++);
          used_variables.insert(dep);
          expr += "math.sin("s + make_var(dep) + ")"s;
        }
        break;

      case NodeType::COS:
        {
          RT_ASSERT(num_deps == -1, "Expected one dependency, but got " << -num_deps);
          const auto dep = *(dep_it++);
          used_variables.insert(dep);
          expr += "math.cos("s + make_var(dep) + ")"s;
        }
        break;

      default:
        RT_TODO("Operation `" << op << "` not implemented yet.");
    }

    expressions.push_back(std::move(expr));
  }
  // - Generate expressions ----------------------------------------------------

  // - Write code --------------------------------------------------------------
  std::ofstream out(filename);
  if (!out) {
    throw std::runtime_error(RT_ERROR_LOC() + ": Could not open file `"s + filename + "`: "s +
                             std::strerror(errno));
  }

  out << "import math\n\n\n";

  out << "def f(";
  for (int64_t id : IteratorReverser(input_variables)) {
    out << make_var(id) << ',';
  }
  out << "):\n";

  for (const auto& expr : IteratorReverser(expressions)) {
    out << single_indent << expr << '\n';
  }
  out << '\n' << single_indent << "return ";
  for (int64_t id : IteratorReverser(possible_output_variables)) {
    if (!used_variables.contains(id)) {
      out << make_var(id) << ", ";
    }
  }
  out << "\n\n\n";

  out << "def main():\n";
  out << single_indent << "print(f\"{f(";
  for (const auto& val : IteratorReverser(input_values)) {
    out << val << ',';
  }
  out << ") = }\")\n\n\n";

  out << "if __name__ == \"__main__\":\n";
  out << single_indent << "main()\n";
  // - Write code --------------------------------------------------------------
}

}  // namespace RT

#endif  // RT_TO_PYTHON_HPP_