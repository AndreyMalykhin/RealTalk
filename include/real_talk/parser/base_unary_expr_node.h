
#ifndef _REAL_TALK_PARSER_BASE_UNARY_EXPR_NODE_H_
#define _REAL_TALK_PARSER_BASE_UNARY_EXPR_NODE_H_

#include <memory>
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class BaseUnaryExprNode final {
 public:
  BaseUnaryExprNode(
      const real_talk::lexer::TokenInfo &op_token,
      std::unique_ptr<ExprNode> operand)
      : op_token_(op_token),
        operand_(move(operand)) {
    assert(operand_);
  }

  const real_talk::lexer::TokenInfo &GetOpToken() const {
    return op_token_;
  }

  const std::unique_ptr<ExprNode> &GetOperand() const {
    return operand_;
  }

  friend bool operator==(const BaseUnaryExprNode &lhs,
                         const BaseUnaryExprNode &rhs) {
    return lhs.op_token_ == rhs.op_token_
        && *(lhs.operand_) == *(rhs.operand_);
  }

 private:
  real_talk::lexer::TokenInfo op_token_;
  std::unique_ptr<ExprNode> operand_;
};
}
}

#endif
