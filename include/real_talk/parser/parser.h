
#ifndef _REAL_TALK_PARSER_PARSER_H_
#define _REAL_TALK_PARSER_PARSER_H_

#include <string>
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/program_node.h"

namespace real_talk {
namespace lexer {

class Lexer;
}

namespace parser {

class Parser {
 public:
  class UnexpectedTokenError: public std::runtime_error {
   public:
    UnexpectedTokenError(
        const real_talk::lexer::TokenInfo &token,
        const std::string &msg);
    const real_talk::lexer::TokenInfo &GetToken() const;
    friend bool operator==(const Parser::UnexpectedTokenError &lhs,
                           const Parser::UnexpectedTokenError &rhs);
    friend std::ostream &operator<<(
        std::ostream &stream, const Parser::UnexpectedTokenError &error);

   private:
    real_talk::lexer::TokenInfo token_;
  };

  virtual ~Parser() {}

  /**
   * @throws real_talk::parser::Parser::UnexpectedTokenError
   * @throws real_talk::lexer::Lexer::UnexpectedCharError
   * @throws real_talk::util::IOError
   */
  virtual std::unique_ptr<ProgramNode> Parse(
      real_talk::lexer::Lexer *lexer) = 0;
};
}
}
#endif
