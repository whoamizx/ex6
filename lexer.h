#ifndef LEX_H
#define LEX_H
#include <string>
#include <unordered_map>

enum TokenType {
  NUL,  // ' ', '\n', '\t', '\r', '\v', '\f'
  IDENT,
  NUMBER,
  PLUS,
  MINUS,
  TIMES,
  SLASH,
  EQL,
  NEQ,
  LSS,
  LEQ,
  GTR,
  GEQ,
  BECOMES,  // +, -, *, /, =, #, <, <=, >, >=, :=
  LPAREN,
  RPAREN,
  COMMA,
  SEMICOLON,
  PERIOD,  // (, ), ,, ;, .
  BEGINSYM,
  CALLSYM,
  CONSTSYM,
  DOSYM,
  ENDSYM,
  IFSYM,
  ODDSYM,
  PROCEDURESYM,
  READSYM,
  THENSYM,
  VARSYM,
  WHILESYM,
  WRITESYM,
  UNKNOWN
};

const std::string TokenTypeNames[] = {
    "NUL",     "IDENT",    "NUMBER",  "PLUS",     "MINUS",    "TIMES",  "SLASH",  "EQL",    "NEQ",
    "LSS",     "LEQ",      "GTR",     "GEQ",      "BECOMES",  "LPAREN", "RPAREN", "COMMA",  "SEMICOLON",
    "PERIOD",  "BEGINSYM", "CALLSYM", "CONSTSYM", "DOSYM",    "ENDSYM", "IFSYM",  "ODDSYM", "PROCEDURESYM",
    "READSYM", "THENSYM",  "VARSYM",  "WHILESYM", "WRITESYM", "UNKNOWN"};

class Token {
 public:
  TokenType type;
  std::string text;
};

std::ostream &operator<<(std::ostream &os, const Token &token);

struct Reserved {
  TokenType type;
  std::string token_type_name;
};
struct Symbol {
  TokenType type;
  std::string token_type_name;
};
class Lexer {
  std::string input;
  char peek;
  int pos{};
  int lastMatchPos{};
  std::unordered_map<std::string, Reserved> reserved_words = {
      {"begin", {BEGINSYM, "BEGINSYM"}}, {"call", {CALLSYM, "CALLSYM"}},
      {"const", {CONSTSYM, "CONSTSYM"}}, {"do", {DOSYM, "DOSYM"}},
      {"end", {ENDSYM, "ENDSYM"}},       {"if", {IFSYM, "IFSYM"}},
      {"odd", {ODDSYM, "ODDSYM"}},       {"procedure", {PROCEDURESYM, "PROCEDURESYM"}},
      {"read", {READSYM, "READSYM"}},    {"then", {THENSYM, "THENSYM"}},
      {"var", {VARSYM, "VARSYM"}},       {"while", {WHILESYM, "WHILESYM"}},
      {"write", {WRITESYM, "WRITESYM"}}};
  std::unordered_map<std::string, Symbol> symbols = {{"+", {PLUS, "PLUS"}},     {"-", {MINUS, "MINUS"}},
                                                     {"*", {TIMES, "TIMES"}},   {"/", {SLASH, "SLASH"}},
                                                     {"=", {EQL, "EQL"}},       {"#", {NEQ, "NEQ"}},
                                                     {"<", {LSS, "LSS"}},       {"<=", {LEQ, "LEQ"}},
                                                     {">", {GTR, "GTR"}},       {">=", {GEQ, "GEQ"}},
                                                     {"(", {LPAREN, "LPAREN"}}, {")", {RPAREN, "RPAREN"}},
                                                     {",", {COMMA, "COMMA"}},   {";", {SEMICOLON, "SEMICOLON"}},
                                                     {".", {PERIOD, "PERIOD"}}, {":=", {BECOMES, "BECOMES"}}};

 public:
  Lexer(const std::string &input);

  void advance();

  [[nodiscard]] bool is_pos_symbol(char peek) const;
  [[nodiscard]] bool is_pos_double_symbol(char peek) const;
  Token nextToken();
};
#endif
