
#ifndef _REAL_TALK_PARSER_STRING_NODE_H_
#define _REAL_TALK_PARSER_STRING_NODE_H_

#include <string>
#include "real_talk/parser/lit_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class StringNode: public LitNode {
 public:
  explicit StringNode(const real_talk::lexer::TokenInfo &token): token_(token) {
  }

  const real_talk::lexer::TokenInfo &GetToken() const {
    return token_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitString(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const StringNode &string_node = static_cast<const StringNode&>(node);
    return token_ == string_node.token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << token_.GetValue();
  }

  real_talk::lexer::TokenInfo token_;
};
}
}

#endif
