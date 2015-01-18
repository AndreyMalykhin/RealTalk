
#ifndef _REAL_TALK_PARSER_UNARY_EXPR_NODE_H_
#define _REAL_TALK_PARSER_UNARY_EXPR_NODE_H_

#include <memory>
#include "real_talk/parser/expr_node.h"

namespace real_talk {
namespace lexer {

class TokenInfo;
}

namespace parser {

class UnaryExprNode: public ExprNode {
 public:
  virtual const real_talk::lexer::TokenInfo &GetOpToken() const = 0;
  virtual const std::unique_ptr<ExprNode> &GetOperand() const = 0;
};
}
}
#endif
