#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "lexer.h"

namespace fs = std::filesystem;

void ToLowerString(std::string& input) {
  for (char& c : input) 
  {
    c = std::tolower(static_cast<unsigned char>(c));
  }
}

void process_file(const fs::path& input_file, const fs::path& output_file) {
  std::ifstream in(input_file);
  if (!in) {
    std::cerr << "Failed to open input file: " << input_file << "\n";
    return;
  }
  auto content = std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());

  ToLowerString(content);

  Lexer lexer(content);
  lexer.CheckGrammar();
}

void process_directory(const fs::path& input_folder, const fs::path& output_folder) {
  if (!exists(output_folder)) {
    create_directory(output_folder);
  }

  for (const auto& entry : fs::directory_iterator(input_folder)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    const fs::path& input_file = entry.path();
    fs::path output_file = output_folder / (input_file.filename().string() + "_result.txt");

    process_file(input_file, output_file);
    std::cout << "Processed: " << input_file << std::endl;
  }
}

int main() {
  process_directory("../test3", "../test3_out");
  return 0;
}