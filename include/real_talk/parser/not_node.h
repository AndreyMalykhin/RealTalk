
#ifndef _REAL_TALK_PARSER_NOT_NODE_H_
#define _REAL_TALK_PARSER_NOT_NODE_H_

#include <memory>
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class NotNode: public ExprNode {
 public:
  explicit NotNode(
      const real_talk::lexer::TokenInfo &token,
      std::unique_ptr<ExprNode> operand)
      : token_(token),
        operand_(move(operand)) {
    assert(operand_);
  }

  const real_talk::lexer::TokenInfo &GetToken() const {
    return token_;
  }

  const std::unique_ptr<ExprNode> &GetOperand() const {
    return operand_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitNot(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const NotNode &not_node = static_cast<const NotNode&>(node);
    return token_ == not_node.token_ && *operand_ == *(not_node.operand_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << token_.GetValue() << *operand_;
  }

  real_talk::lexer::TokenInfo token_;
  std::unique_ptr<ExprNode> operand_;
};
}
}

#endif
