#include "Parser.h"

#include <iostream>

void Parser::error(const std::string &message) {
  throw std::runtime_error("语法错误: " + message + " at position " + std::to_string(pos));
}

void Parser::CheckGrammar() {
  currentToken = nextToken();
  try {
    expression();  //statement();  只处理单个表达式
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

std::string Parser::expression() {
  std::string result = term();
  while (currentToken.type == PLUS || currentToken.type == MINUS) {
    std::string op = currentToken.type == PLUS ? "+" : "-";
    currentToken = nextToken();
    std::string right = term();
    std::string temp = quad_gen.new_temp();
    quad_gen.add_quadruple(Quadruple(op, result, right, temp));
    result = temp;
  }
  return result;
}

std::string Parser::term() {
  std::string result = factor();
  while (currentToken.type == TIMES || currentToken.type == SLASH) {
    std::string op = currentToken.type == TIMES ? "*" : "/";
    currentToken = nextToken();
    std::string right = factor();
    std::string temp = quad_gen.new_temp();
    quad_gen.add_quadruple(Quadruple(op, result, right, temp));
    result = temp;
  }
  return result;
}

std::string Parser::factor() {
  if (currentToken.type == IDENT || currentToken.type == NUMBER) {
    std::string result = currentToken.text;
    currentToken = nextToken();
    return result;
  } else if (pos_match({LPAREN})) {
    std::string result = expression();
    match({RPAREN});
    return result;
  } else {
    error("Unexpected token: " + TokenTypeNames[currentToken.type]+":"+currentToken.text);
    return "";
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
