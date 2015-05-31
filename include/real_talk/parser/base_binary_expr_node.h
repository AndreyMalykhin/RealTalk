
#ifndef _REAL_TALK_PARSER_BASE_BINARY_EXPR_NODE_H_
#define _REAL_TALK_PARSER_BASE_BINARY_EXPR_NODE_H_

#include <memory>
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class BaseBinaryExprNode final {
 public:
  BaseBinaryExprNode(
      const real_talk::lexer::TokenInfo &op_token,
      std::unique_ptr<ExprNode> left_operand,
      std::unique_ptr<ExprNode> right_operand)
      : op_token_(op_token),
      left_operand_(move(left_operand)),
      right_operand_(move(right_operand)) {
    assert(left_operand_);
    assert(right_operand_);
  }

  const real_talk::lexer::TokenInfo &GetStartToken() const {
    return left_operand_->GetStartToken();
  }

  const real_talk::lexer::TokenInfo &GetOpToken() const {
    return op_token_;
  }

  const std::unique_ptr<ExprNode> &GetLeftOperand() const {
    return left_operand_;
  }

  const std::unique_ptr<ExprNode> &GetRightOperand() const {
    return right_operand_;
  }

  friend std::ostream &operator<<(std::ostream &stream,
                                 const BaseBinaryExprNode &node) {
    stream << '(' << *(node.left_operand_) << ' ' << node.op_token_.GetValue()
           << ' ' << *(node.right_operand_) << ')';
    return stream;
  }

  friend bool operator==(const BaseBinaryExprNode &lhs,
                         const BaseBinaryExprNode &rhs) {
    return lhs.op_token_ == rhs.op_token_
        && *(lhs.left_operand_) == *(rhs.left_operand_)
        && *(lhs.right_operand_) == *(rhs.right_operand_);
  }

 private:
  real_talk::lexer::TokenInfo op_token_;
  std::unique_ptr<ExprNode> left_operand_;
  std::unique_ptr<ExprNode> right_operand_;
};
}
}

#endif
