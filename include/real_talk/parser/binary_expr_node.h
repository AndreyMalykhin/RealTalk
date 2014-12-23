
#ifndef _REAL_TALK_PARSER_BINARY_EXPR_NODE_H_
#define _REAL_TALK_PARSER_BINARY_EXPR_NODE_H_

#include <memory>
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class BinaryExprNode final {
 public:
  BinaryExprNode(
      const real_talk::lexer::TokenInfo &token,
      std::unique_ptr<ExprNode> left_operand,
      std::unique_ptr<ExprNode> right_operand)
      : token_(token),
      left_operand_(move(left_operand)),
      right_operand_(move(right_operand)) {
    assert(left_operand_);
    assert(right_operand_);
  }

  const real_talk::lexer::TokenInfo &GetToken() const {
    return token_;
  }

  const std::unique_ptr<ExprNode> &GetLeftOperand() const {
    return left_operand_;
  }

  const std::unique_ptr<ExprNode> &GetRightOperand() const {
    return right_operand_;
  }

  friend std::ostream &operator<<(std::ostream &stream,
                                 const BinaryExprNode &node) {
    stream << '(' << *(node.left_operand_) << ' ' << node.token_.GetValue()
           << ' ' << *(node.right_operand_) << ')';
    return stream;
  }

  friend bool operator==(const BinaryExprNode &lhs, const BinaryExprNode &rhs) {
    return lhs.token_ == rhs.token_
        && *(lhs.left_operand_) == *(rhs.left_operand_)
        && *(lhs.right_operand_) == *(rhs.right_operand_);
  }

 private:
  real_talk::lexer::TokenInfo token_;
  std::unique_ptr<ExprNode> left_operand_;
  std::unique_ptr<ExprNode> right_operand_;
};
}
}

#endif
