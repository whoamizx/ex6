#ifndef PARSER_H
#define PARSER_H
#include <unordered_set>
#include <string>
#include <vector>

#include "lexer.h"
#include "quadruple.h"

struct ExprResult {
    std::string result;
    std::vector<Quadruple> quads;
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
    QuadrupleGenerator quad_gen;
    std::vector<Quadruple> quadruples;

    void match(const std::unordered_set<TokenType> &expected);
    bool pos_match(const std::unordered_set<TokenType> &expected);

    ExprResult expression();
    ExprResult term();
    ExprResult factor();
    void error(const std::string &message);
    void statement_list();
    void statement();
    void condition();

    std::vector<Token> tokens;
};

#endif //PARSER_H
