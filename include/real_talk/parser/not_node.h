
#ifndef _REAL_TALK_PARSER_NOT_NODE_H_
#define _REAL_TALK_PARSER_NOT_NODE_H_

#include "real_talk/parser/unary_expr_node.h"
#include "real_talk/parser/base_unary_expr_node.h"

namespace real_talk {
namespace parser {

class NotNode: public UnaryExprNode {
 public:
  NotNode(
      const real_talk::lexer::TokenInfo &op_token,
      std::unique_ptr<ExprNode> operand)
      : unary_expr_(op_token, move(operand)) {
  }

  virtual const real_talk::lexer::TokenInfo &GetOpToken() const override {
    return unary_expr_.GetOpToken();
  }

  virtual const std::unique_ptr<ExprNode> &GetOperand() const override {
    return unary_expr_.GetOperand();
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitNot(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const NotNode &rhs = static_cast<const NotNode&>(node);
    return unary_expr_ == rhs.unary_expr_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << unary_expr_.GetOpToken().GetValue()
           << *(unary_expr_.GetOperand());
  }

  BaseUnaryExprNode unary_expr_;
};
}
}

#endif
