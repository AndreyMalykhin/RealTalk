
#ifndef _REAL_TALK_PARSER_PARSER_FACTORY_H_
#define _REAL_TALK_PARSER_PARSER_FACTORY_H_

#include <memory>

namespace real_talk {
namespace lexer {

class Lexer;
}

namespace parser {

class Parser;

class ParserFactory {
 public:
  virtual ~ParserFactory() {}
  virtual std::unique_ptr<Parser> Create(
      std::unique_ptr<real_talk::lexer::Lexer> lexer) const = 0;
};
}
}
#endif
