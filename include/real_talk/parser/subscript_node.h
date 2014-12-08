
#ifndef _REAL_TALK_PARSER_SUBSCRIPT_NODE_H_
#define _REAL_TALK_PARSER_SUBSCRIPT_NODE_H_

#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class SubscriptNode: public ExprNode {
 public:
  SubscriptNode(
      const real_talk::lexer::TokenInfo &start_token,
      const real_talk::lexer::TokenInfo &end_token,
      std::unique_ptr<ExprNode> operand,
      std::unique_ptr<ExprNode> index)
      : start_token_(start_token),
        end_token_(end_token),
        operand_(move(operand)),
        index_(move(index)) {
    assert(operand_);
    assert(index_);
  }

  const real_talk::lexer::TokenInfo &GetStartToken() const {
    return start_token_;
  }

  const real_talk::lexer::TokenInfo &GetEndToken() const {
    return end_token_;
  }

  const std::unique_ptr<ExprNode> &GetOperand() const {
    return operand_;
  }

  const std::unique_ptr<ExprNode> &GetIndex() const {
    return index_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const SubscriptNode &subscript_node =
        static_cast<const SubscriptNode&>(node);
    return start_token_ == subscript_node.start_token_
        && end_token_ == subscript_node.end_token_
        && *operand_ == *(subscript_node.operand_)
        && *index_ == *(subscript_node.index_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *operand_ << start_token_.GetValue() << *index_
           << end_token_.GetValue();
  }

  real_talk::lexer::TokenInfo start_token_;
  real_talk::lexer::TokenInfo end_token_;
  std::unique_ptr<ExprNode> operand_;
  std::unique_ptr<ExprNode> index_;
};
}
}

#endif
