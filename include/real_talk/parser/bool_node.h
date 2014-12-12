
#ifndef _REAL_TALK_PARSER_BOOL_NODE_H_
#define _REAL_TALK_PARSER_BOOL_NODE_H_

#include <string>
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class BoolNode: public ExprNode {
 public:
  explicit BoolNode(const real_talk::lexer::TokenInfo &token): token_(token) {
  }

  const real_talk::lexer::TokenInfo &GetToken() const {
    return token_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitBool(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const BoolNode &bool_node = static_cast<const BoolNode&>(node);
    return token_ == bool_node.token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << token_.GetValue();
  }

  real_talk::lexer::TokenInfo token_;
};
}
}

#endif
