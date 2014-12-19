
#ifndef _REAL_TALK_LEXER_SIMPLE_LEXER_FACTORY_H_
#define _REAL_TALK_LEXER_SIMPLE_LEXER_FACTORY_H_

#include "real_talk/lexer/lexer_factory.h"

namespace real_talk {
namespace lexer {

class SimpleLexerFactory: public LexerFactory {
 public:
  virtual std::unique_ptr<Lexer> Create(std::istream &stream) const override;
};
}
}
#endif
