#ifndef SAVE_TO_DOT_HPP_
#define SAVE_TO_DOT_HPP_

#include <filesystem>
#include <iostream>
#include <string>

#include "Graph.hpp"

template <typename RT_Type>
void save_to_dot(const char* cpp_source_name, const RT::GraphToDotOptions& opt = {}) {
  using namespace std::string_literals;

  const auto out_file =
      "graphs/"s + std::filesystem::path(cpp_source_name).stem().c_str() + ".dot"s;
  try {
    RT_Type::graph().to_dot(out_file, opt);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    std::exit(1);
  }
  std::cout << "Wrote Graph to `" << out_file << "`.\n";
}

#endif  // SAVE_TO_DOT_HPP_