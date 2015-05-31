
#ifndef _REAL_TALK_PARSER_EXPR_NODE_H_
#define _REAL_TALK_PARSER_EXPR_NODE_H_

#include "real_talk/parser/node.h"

namespace real_talk {
namespace lexer {

class TokenInfo;
}

namespace parser {

class ExprNode: public Node {
 public:
  virtual const real_talk::lexer::TokenInfo &GetStartToken() const = 0;
};
}
}

#endif
