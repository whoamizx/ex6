#include "Parser.h"

#include <iostream>

void Parser::error(const std::string &message) {
  throw std::runtime_error("语法错误: " + message + " at position " + std::to_string(pos));
}

void Parser::CheckGrammar() {
  currentToken = nextToken();
  try {
    ExprResult result = expression(1);  // 从1开始计数临时变量
    quadruples = result.quads;
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
    expression(0);
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
    expression(0);
    while (pos_match({COMMA})) {
      expression(0);
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

ExprResult Parser::expression(int start_temp) {
  ExprResult left = term(start_temp);
  std::vector<Quadruple> quads = left.quads;
  int current_temp = start_temp + left.temp_count;

  while (currentToken.type == PLUS || currentToken.type == MINUS) {
    std::string op = currentToken.type == PLUS ? "+" : "-";
    currentToken = nextToken();
    ExprResult right = term(current_temp);
    quads.insert(quads.end(), right.quads.begin(), right.quads.end()); // 插入右边的四元式
    current_temp += right.temp_count; // 更新当前非终结符节点临时变量计数
    std::string temp = "t" + std::to_string(current_temp);
    quads.push_back(Quadruple(op, left.result, right.result, temp)); // 生成新的四元式
    left = {temp, quads, right.temp_count + 1}; // 将已完成操作的部分视为左边
    current_temp += 1;  // +1 是因为当前操作生成了一个新的临时变量
  }
  
  return {left.result, quads, current_temp - start_temp};
}

ExprResult Parser::term(int start_temp) {
  ExprResult left = factor(start_temp);
  std::vector<Quadruple> quads = left.quads;
  int current_temp = start_temp + left.temp_count;

  while (currentToken.type == TIMES || currentToken.type == SLASH) {
    std::string op = currentToken.type == TIMES ? "*" : "/";
    currentToken = nextToken();
    ExprResult right = factor(current_temp);
    quads.insert(quads.end(), right.quads.begin(), right.quads.end());
    current_temp += right.temp_count;
    std::string temp = "t" + std::to_string(current_temp);
    quads.push_back(Quadruple(op, left.result, right.result, temp));
    left = {temp, quads, right.temp_count + 1};
    current_temp += 1;  // +1 是因为当前操作生成了一个新的临时变量
  }
  
  return {left.result, quads, current_temp - start_temp};
}

ExprResult Parser::factor(int start_temp) {
  if (currentToken.type == IDENT || currentToken.type == NUMBER) {
    std::string result = currentToken.text;
    currentToken = nextToken();
    return {result, {}, 0};  // 没有使用临时变量
  } else if (pos_match({LPAREN})) {
    ExprResult result = expression(start_temp);
    match({RPAREN});
    return result;
  } else {
    error("Unexpected token: " + TokenTypeNames[currentToken.type]+":"+currentToken.text);
    return {"", {}, 0};
  }
}

void Parser::condition() {
  if (pos_match({ODDSYM})) {
    expression(0);
  } else {
    expression(0);
    match({EQL, NEQ, LSS, LEQ, GTR, GEQ});
    expression(0);
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
