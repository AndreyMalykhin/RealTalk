
#ifndef _REAL_TALK_PARSER_BINARY_EXPR_NODE_H_
#define _REAL_TALK_PARSER_BINARY_EXPR_NODE_H_

#include <memory>
#include "real_talk/parser/node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class BinaryExprNode final: public Node {
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

  virtual void Print(std::ostream &stream) const override {
    stream << '(' << *left_operand_ << ' ' << token_.GetValue() << ' '
           << *right_operand_ << ')';
  }

  virtual bool IsEqual(const Node &node) const override {
    const BinaryExprNode &bin_node = static_cast<const BinaryExprNode&>(node);
    return token_ == bin_node.token_
        && *left_operand_ == *(bin_node.left_operand_)
        && *right_operand_ == *(bin_node.right_operand_);
  }

 private:
  real_talk::lexer::TokenInfo token_;
  std::unique_ptr<ExprNode> left_operand_;
  std::unique_ptr<ExprNode> right_operand_;
};
}
}

#endif
