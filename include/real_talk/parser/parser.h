
#ifndef _REAL_TALK_PARSER_PARSER_H_
#define _REAL_TALK_PARSER_PARSER_H_

#include <string>
#include <memory>
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/program_node.h"

namespace real_talk {
namespace parser {

class UnexpectedTokenError: public std::runtime_error {
 public:
  UnexpectedTokenError(
      const real_talk::lexer::TokenInfo &token,
      const std::string &msg);
  const real_talk::lexer::TokenInfo &GetToken() const;

 private:
  real_talk::lexer::TokenInfo token_;
};

class Parser {
 public:
  virtual ~Parser() {}

  /**
   * @throws real_talk::parser::UnexpectedTokenError
   * @throws real_talk::lexer::UnexpectedCharError
   * @throws real_talk::util::IOError
   */
  virtual std::shared_ptr<ProgramNode> Parse() = 0;
};
}
}
#endif
