
#ifndef _REAL_TALK_PARSER_OR_NODE_H_
#define _REAL_TALK_PARSER_OR_NODE_H_

#include "real_talk/parser/binary_expr_node.h"
#include "real_talk/parser/base_binary_expr_node.h"

namespace real_talk {
namespace parser {

class OrNode: public BinaryExprNode {
 public:
  OrNode(
      const real_talk::lexer::TokenInfo &token,
      std::unique_ptr<ExprNode> left_operand,
      std::unique_ptr<ExprNode> right_operand)
      : binary_expr_(token, move(left_operand), move(right_operand)) {}

  virtual const real_talk::lexer::TokenInfo &GetStartToken() const override {
    return binary_expr_.GetStartToken();
  }

  const real_talk::lexer::TokenInfo &GetOpToken() const override {
    return binary_expr_.GetOpToken();
  }

  const std::unique_ptr<ExprNode> &GetLeftOperand() const override {
    return binary_expr_.GetLeftOperand();
  }

  const std::unique_ptr<ExprNode> &GetRightOperand() const override {
    return binary_expr_.GetRightOperand();
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitOr(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    return binary_expr_ == static_cast<const OrNode&>(node).binary_expr_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << binary_expr_;
  }

  BaseBinaryExprNode binary_expr_;
};
}
}

#endif
