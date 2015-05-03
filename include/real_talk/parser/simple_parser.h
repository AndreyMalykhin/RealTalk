
#ifndef _REAL_TALK_PARSER_SIMPLE_PARSER_H_
#define _REAL_TALK_PARSER_SIMPLE_PARSER_H_

#include <memory>
#include "real_talk/parser/parser.h"

namespace real_talk {
namespace lexer {

class Lexer;
}

namespace parser {

class SimpleParser: public Parser {
 public:
  explicit SimpleParser(std::unique_ptr<real_talk::lexer::Lexer> lexer);
  virtual ~SimpleParser() override;
  virtual std::shared_ptr<ProgramNode> Parse() override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
#endif
