
#ifndef _REAL_TALK_PARSER_SIMPLE_PARSER_H_
#define _REAL_TALK_PARSER_SIMPLE_PARSER_H_

#include <memory>
#include "real_talk/parser/parser.h"

namespace real_talk {
namespace parser {

class SimpleParser: public Parser {
 public:
  SimpleParser();
  virtual ~SimpleParser() override;
  virtual std::unique_ptr<ProgramNode> Parse(
      real_talk::lexer::Lexer *lexer) override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
#endif
