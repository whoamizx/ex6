#ifndef PARSER_H
#define PARSER_H
#include <unordered_set>

#include "lexer.h"

class Parser {
  public:
    explicit Parser(const std::vector<Token> &tokens)
      : pos(-1), tokens(tokens) {
    }

    void CheckGrammar();

  private:
    Token nextToken();

    int pos{};
    Token currentToken{};

    void match(const std::unordered_set<TokenType> &expected);

    bool pos_match(const std::unordered_set<TokenType> &expected);

    void expression();

    void term();

    void factor();

    void error(const std::string &message);

    void statement_list();

    void statement();

    void condition();

    std::vector<Token> tokens;
};


#endif //PARSER_H
