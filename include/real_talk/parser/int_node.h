
#ifndef _REAL_TALK_PARSER_INT_NODE_H_
#define _REAL_TALK_PARSER_INT_NODE_H_

#include "real_talk/parser/expr_node.h"
#include "real_talk/parser/node_visitor.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class IntNode: public ExprNode {
 public:
  explicit IntNode(const real_talk::lexer::TokenInfo &token): token_(token) {
  }

  const real_talk::lexer::TokenInfo &GetToken() const {
    return token_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitInt(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const IntNode &int_node = static_cast<const IntNode&>(node);
    return token_ == int_node.token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << token_.GetValue();
  }

  real_talk::lexer::TokenInfo token_;
};
}
}

#endif
