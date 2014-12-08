
#ifndef _REAL_TALK_LEXER_LEXER_H_
#define _REAL_TALK_LEXER_LEXER_H_

namespace real_talk {
namespace lexer {

class TokenInfo;

class Lexer {
 public:
  virtual TokenInfo GetNextToken() = 0;
  virtual ~Lexer() {}
};
}
}

#endif
