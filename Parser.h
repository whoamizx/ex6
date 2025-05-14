#ifndef PARSER_H
#define PARSER_H
#include <unordered_set>
#include <string>

#include "lexer.h"
#include "quadruple.h"

class Parser {
  public:
    explicit Parser(const std::vector<Token> &tokens)
      : pos(-1), tokens(tokens) {
    }

    void CheckGrammar();
    const std::vector<Quadruple>& get_quadruples() const { return quad_gen.get_quadruples(); }

  private:
    Token nextToken();
    int pos{};
    Token currentToken{};
    QuadrupleGenerator quad_gen;

    void match(const std::unordered_set<TokenType> &expected);
    bool pos_match(const std::unordered_set<TokenType> &expected);

    std::string expression();
    std::string term();
    std::string factor();
    void error(const std::string &message);
    void statement_list();
    void statement();
    void condition();

    std::vector<Token> tokens;
};

#endif //PARSER_H
