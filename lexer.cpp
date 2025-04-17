#include "lexer.h"
std::ostream& operator<<(std::ostream& os, const Token& token) {
  os << std::string("(") << TokenTypeNames[token.type] << std::string(", ") << token.text << std::string(")\n");
  return os;
}
Lexer::Lexer(const std::string& input) {
  this->input = input;
  peek = input[pos];
}
void Lexer::advance() {
  pos++;
  if (pos >= input.length()) {
    peek = '\0';
  } else {
    peek = input[pos];
  }
}
bool Lexer::is_pos_symbol(const char peek) const {
  return std::string("+-*/#=<>:();,.").find(peek) != std::string::npos;
}
bool Lexer::is_pos_double_symbol(const char peek) const { return std::string("<>:").find(peek) != std::string::npos; }
Token Lexer::nextToken() {

  if (pos >= input.length()) {
    return Token(UNKNOWN, "EOF");
  }
  Token token;
  while (isspace(peek)) {
    advance();
  }
  lastMatchPos = pos;
  if (peek == '/' && pos + 1 < input.length() && input[pos+1] == '*') {
    advance();
    advance();
    
    while (true) {
        if (peek == '\0') {
            return Token(UNKNOWN, "Unterminated comment");
        }
        if (peek == '*' && pos + 1 < input.length() && input[pos+1] == '/') {
            advance();
            advance();
            break;
        }
        advance();
    }
    
    return nextToken();
  }
  if (isdigit(peek)) {
    while (isdigit(peek)) 
    {
      advance();
    }
    if (peek == '.') 
    {
      advance();
      while (isdigit(peek))
      {
        advance();
      }
      token = Token(NUMBER, input.substr(lastMatchPos, pos - lastMatchPos));
    }
    else if (isalpha(peek)) {
      while (peek != '\0' && !isspace(peek)) 
      {
        advance();
      }
      token = Token(UNKNOWN, input.substr(lastMatchPos, pos - lastMatchPos));
      return token;
    }
    else 
    {
      token = Token(NUMBER, input.substr(lastMatchPos, pos - lastMatchPos));
    }
  } else if (isalpha(peek)) {
    while (isalpha(peek)) {
      advance();
    }
    while (isalnum(peek)) {
      advance();
    }
    auto word = input.substr(lastMatchPos, pos - lastMatchPos);
    if (const auto reserved_word = reserved_words.find(word); reserved_word != reserved_words.end()) {
      token = Token(reserved_word->second.type, reserved_word->second.token_type_name);
    } else {
      token = Token(IDENT, input.substr(lastMatchPos, pos - lastMatchPos));
    }
  } else if (is_pos_symbol(peek)) {
    if (!is_pos_double_symbol(peek)) {
      token = Token(symbols[std::string(1, peek)].type, symbols[std::string(1, peek)].token_type_name);
      advance();
    } else {
      const std::string peek1(1, peek);
      advance();
      if (const auto symbol = symbols.find(peek1 + peek); symbol == symbols.end() || peek == '\0') {
        token = Token(symbols[peek1].type, symbols[peek1].token_type_name);
        if (peek1 == ":") {
          token = Token(UNKNOWN, ":");
        }
      } else {
        token = Token(symbols[symbol->first].type, symbols[symbol->first].token_type_name);
        advance();
      }
    }
  } else {
    token = Token(UNKNOWN, {peek});
    advance();
  }
  return token;
}
