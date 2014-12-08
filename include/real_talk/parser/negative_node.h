
#ifndef _REAL_TALK_PARSER_NEGATIVE_NODE_H_
#define _REAL_TALK_PARSER_NEGATIVE_NODE_H_

#include <memory>
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class NegativeNode: public ExprNode {
 public:
  NegativeNode(
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

 private:
  virtual bool IsEqual(const Node &node) const override {
    const NegativeNode &rhs = static_cast<const NegativeNode&>(node);
    return token_ == rhs.token_
        && *operand_ == *(rhs.operand_);
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
