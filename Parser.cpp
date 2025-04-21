#include "Parser.h"

#include <iostream>

void Parser::error(const std::string &message) {
  throw std::runtime_error("语法错误: " + message + " at position " + std::to_string(pos));
}

void Parser::CheckGrammar() {
  currentToken = nextToken();
  try {
    statement();
    std::cout << "语法正确" << std::endl;
  } catch (const std::exception &e) {
    std::cout << e.what() << std::endl;
  }
}

Token Parser::nextToken() {
  pos++;
  return tokens[pos];
}

void Parser::statement() {
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

void Parser::statement_list() {
  statement();
  while (pos_match({SEMICOLON})) {
    statement();
  }
}

void Parser::expression() {
  term();
  while (pos_match({PLUS, MINUS})) {
    term();
  }
}

void Parser::term() {
  factor();
  while (pos_match({TIMES, SLASH})) {
    factor();
  }
}

void Parser::factor() {
  if (pos_match({IDENT, NUMBER})) {
  } else if (pos_match({LPAREN})) {
    expression();
    match({RPAREN});
  } else {
    error("Unexpected token: " + TokenTypeNames[currentToken.type]+":"+currentToken.text);
  }
}

void Parser::condition() {
  if (pos_match({ODDSYM})) {
    expression();
  } else {
    expression();
    match({EQL, NEQ, LSS, LEQ, GTR, GEQ});
    expression();
  }
}

void Parser::match(const std::unordered_set<TokenType> &expected) {
  if (expected.contains(currentToken.type)) {
    currentToken = nextToken();
  } else {
    error("Unexpected token: " + TokenTypeNames[currentToken.type]+":"+currentToken.text);
  }
}

bool Parser::pos_match(const std::unordered_set<TokenType> &expected) {
  if (expected.contains(currentToken.type)) {
    currentToken = nextToken();
    return true;
  }
  return false;
}
