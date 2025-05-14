#ifndef PARSER_H
#define PARSER_H
#include <unordered_set>
#include <string>
#include <vector>

#include "lexer.h"

struct Quadruple {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string result;
    Quadruple(std::string op, std::string arg1, std::string arg2, std::string result)
      : op(op), arg1(arg1), arg2(arg2), result(result) {}
};

struct ExprResult {
    std::string result;
    std::vector<Quadruple> quads;
    int temp_count;  // 记录当前分支使用的临时变量数量
};

class Parser {
  public:
    explicit Parser(const std::vector<Token> &tokens)
      : pos(-1), tokens(tokens) {
    }

    void CheckGrammar();
    const std::vector<Quadruple>& get_quadruples() const { return quadruples; }

  private:
    Token nextToken();
    int pos{};
    Token currentToken{};
    std::vector<Quadruple> quadruples;

    void match(const std::unordered_set<TokenType> &expected);
    bool pos_match(const std::unordered_set<TokenType> &expected);

    ExprResult expression(int start_temp);
    ExprResult term(int start_temp);
    ExprResult factor(int start_temp);
    void error(const std::string &message);
    void statement_list();
    void statement();
    void condition();

    std::vector<Token> tokens;
};

#endif //PARSER_H
