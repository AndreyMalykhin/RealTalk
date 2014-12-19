
#ifndef _REAL_TALK_LEXER_LEXER_FACTORY_H_
#define _REAL_TALK_LEXER_LEXER_FACTORY_H_

#include <memory>
#include <iostream>

namespace real_talk {
namespace lexer {

class Lexer;

class LexerFactory {
 public:
  virtual ~LexerFactory() {}
  virtual std::unique_ptr<Lexer> Create(std::istream &stream) const = 0;
};
}
}
#endif
