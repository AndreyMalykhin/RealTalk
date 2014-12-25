
#ifndef _REAL_TALK_PARSER_BINARY_EXPR_NODE_H_
#define _REAL_TALK_PARSER_BINARY_EXPR_NODE_H_

#include <memory>
#include "real_talk/parser/expr_node.h"

namespace real_talk {
namespace lexer {

class TokenInfo;
}

namespace parser {

class BinaryExprNode: public ExprNode {
 public:
  virtual const real_talk::lexer::TokenInfo &GetOpToken() const = 0;
  virtual const std::unique_ptr<ExprNode> &GetLeftOperand() const = 0;
  virtual const std::unique_ptr<ExprNode> &GetRightOperand() const = 0;
};
}
}
#endif
