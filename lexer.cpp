#include "lexer.h"

#include <iostream>
#include <unordered_set>

std::ostream &operator<<(std::ostream &os, const Token &token) {
  os << std::string("(") << TokenTypeNames[token.type] << std::string(", ") << token.text << std::string(")\n");
  return os;
}

Lexer::Lexer(const std::string &input) {
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

bool Lexer::is_pos_double_symbol(const char peek) const {
  return std::string("<>:").find(peek) != std::string::npos;
}

Token Lexer::nextToken() {
  if (pos >= input.length()) {
    return Token(UNKNOWN, "EOF");
  }
  Token token;
  while (isspace(peek)) {
    advance();
  }
  lastMatchPos = pos;
  if (peek == '/' && pos + 1 < input.length() && input[pos + 1] == '*') {
    advance();
    advance();

    while (true) {
      if (peek == '\0') {
        return Token(UNKNOWN, "Unterminated comment");
      }
      if (peek == '*' && pos + 1 < input.length() && input[pos + 1] == '/') {
        advance();
        advance();
        break;
      }
      advance();
    }

    return nextToken();
  }
  if (isdigit(peek)) {
    while (isdigit(peek)) {
      advance();
    }
    if (peek == '.') {
      advance();
      while (isdigit(peek)) {
        advance();
      }
      token = Token(NUMBER, input.substr(lastMatchPos, pos - lastMatchPos));
    } else if (isalpha(peek)) {
      while (peek != '\0' && !isspace(peek)) {
        advance();
      }
      token = Token(UNKNOWN, input.substr(lastMatchPos, pos - lastMatchPos));
      return token;
    } else {
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

void Lexer::error(const std::string &message) {
  throw std::runtime_error("语法错误: " + message + " at position " + std::to_string(pos));
}

void Lexer::CheckGrammar() {
  currentToken = nextToken();
  try {
    statement(); // 检查单个语句
    std::cout << "语法正确" << std::endl;
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

void Lexer::statement() {
  if (pos_match({IDENT})) {
    match({BECOMES});
    expression();
  } else if (pos_match({IFSYM})) {
    condition();
    match({THENSYM});
    statement();
  } else if (pos_match({WHILESYM})) {
    condition();
    match({DOSYM});
    statement();
  } else if (pos_match({CALLSYM})) {
    match({IDENT});
  } else if (pos_match({READSYM})) {
    match({LPAREN});
    match({IDENT});
    while (pos_match({COMMA})) {
      match({IDENT});
    }
    match({RPAREN});
  } else if (pos_match({WRITESYM})) {
    match({LPAREN});
    expression();
    while (pos_match({COMMA})) {
      expression();
    }
    match({RPAREN});
  } else if (pos_match({BEGINSYM})) {
    statement_list();
    while (pos_match({SEMICOLON})) {
      statement_list();
    }
    match({ENDSYM});
  } else {
    error("无效的语句");
  }
}

void Lexer::statement_list() {
  statement();
  while (pos_match({SEMICOLON})) {
    statement();
  }
}

void Lexer::expression() {
  term();
  while (pos_match({PLUS, MINUS})) {
    term();
  }
}

void Lexer::term() {
  factor();
  while (pos_match({TIMES, SLASH})) {
    factor();
  }
}

void Lexer::factor() {
  if (pos_match({IDENT, NUMBER})) {
  } else if (pos_match({LPAREN})) {
    expression();
    match({RPAREN});
  } else {
    error("Unexpected token: " + TokenTypeNames[currentToken.type]+":"+currentToken.text);
  }
}

void Lexer::condition() {
  if (pos_match({ODDSYM})) {
    expression();
  } else {
    expression();
    match({EQL, NEQ, LSS, LEQ, GTR, GEQ});
    expression();
  }
}

void Lexer::match(const std::unordered_set<TokenType> &expected) {
  if (expected.contains(currentToken.type)) {
    currentToken = nextToken();
  } else {
    error("Unexpected token: " + TokenTypeNames[currentToken.type]+":"+currentToken.text);
  }
}

bool Lexer::pos_match(const std::unordered_set<TokenType> &expected) {
  if (expected.contains(currentToken.type)) {
    currentToken = nextToken();
    return true;
  }
  return false;
}
