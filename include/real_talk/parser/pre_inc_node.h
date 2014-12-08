
#ifndef _REAL_TALK_PARSER_PRE_INC_NODE_H_
#define _REAL_TALK_PARSER_PRE_INC_NODE_H_

#include <memory>
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class PreIncNode: public ExprNode {
 public:
  PreIncNode(
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
    const PreIncNode &pre_inc_node = static_cast<const PreIncNode&>(node);
    return token_ == pre_inc_node.token_
        && *operand_ == *(pre_inc_node.operand_);
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
