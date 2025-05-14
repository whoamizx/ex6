#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "lexer.h"
#include "Parser.h"

class Parser;
namespace fs = std::filesystem;

void ToLowerString(std::string &input) {
  for (char &c : input) {
    c = std::tolower(static_cast<unsigned char>(c));
  }
}

void process_file(const fs::path &input_file, const fs::path &output_file) {
  std::ifstream in(input_file);
  if (!in) {
    std::cerr << "Failed to open input file: " << input_file << "\n";
    return;
  }
  auto content = std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());

  ToLowerString(content);

  Lexer lexer(content);
  std::vector<Token> tokens;
  auto token = lexer.nextToken();
  while (!(token.type == UNKNOWN && token.text == "EOF")) {
    tokens.push_back(token);
    token=lexer.nextToken();
  }
  tokens.push_back(token);

  Parser parser(tokens);
  parser.CheckGrammar();

  std::ofstream out(output_file);
  if (!out) {
    std::cerr << "Failed to open output file: " << output_file << "\n";
    return;
  }

  for (const auto& quad : parser.get_quadruples()) {
    out << "(" << quad.op << ", " << quad.arg1 << ", " << quad.arg2 << ", " << quad.result << ")\n";
  }
}

void process_directory(const fs::path &input_folder, const fs::path &output_folder) {
  if (!exists(output_folder)) {
    create_directory(output_folder);
  }

  for (const auto &entry : fs::directory_iterator(input_folder)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    const fs::path &input_file = entry.path();
    fs::path output_file = output_folder / (input_file.filename().string() + "_result.txt");

    process_file(input_file, output_file);
    std::cout << "Processed: " << input_file << std::endl;
  }
}

int main() {
  process_directory("../test5", "../test5_out");
  return 0;
}
