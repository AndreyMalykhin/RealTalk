
#ifndef _REAL_TALK_PARSER_RETURN_VALUE_NODE_H_
#define _REAL_TALK_PARSER_RETURN_VALUE_NODE_H_

#include <cassert>
#include <memory>
#include "real_talk/parser/expr_node.h"
#include "real_talk/parser/node_visitor.h"
#include "real_talk/parser/return_node.h"

namespace real_talk {
namespace parser {

class ReturnValueNode: public ReturnNode {
 public:
  ReturnValueNode(
      const real_talk::lexer::TokenInfo &start_token,
      std::unique_ptr<ExprNode> value,
      const real_talk::lexer::TokenInfo &end_token)
      : start_token_(start_token),
        value_(move(value)),
        end_token_(end_token) {
    assert(value_);
  }

  virtual const real_talk::lexer::TokenInfo &GetStartToken() const override {
    return start_token_;
  }

  virtual const real_talk::lexer::TokenInfo &GetEndToken() const override {
    return end_token_;
  }

  const std::unique_ptr<ExprNode> &GetValue() const {
    return value_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitReturnValue(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ReturnValueNode &rhs = static_cast<const ReturnValueNode&>(node);
    return start_token_ == rhs.start_token_
        && end_token_ == rhs.end_token_
        && *value_ == *(rhs.value_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << start_token_.GetValue() << ' ' << *value_ << end_token_.GetValue();
  }

  real_talk::lexer::TokenInfo start_token_;
  std::unique_ptr<ExprNode> value_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}
#endif
