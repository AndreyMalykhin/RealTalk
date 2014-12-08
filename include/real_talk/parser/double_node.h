
#ifndef _REAL_TALK_PARSER_DOUBLE_NODE_H_
#define _REAL_TALK_PARSER_DOUBLE_NODE_H_

#include <string>
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class DoubleNode: public ExprNode {
 public:
  explicit DoubleNode(const real_talk::lexer::TokenInfo &token): token_(token) {
  }

  const real_talk::lexer::TokenInfo &GetToken() const {
    return token_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const DoubleNode &double_node = static_cast<const DoubleNode&>(node);
    return token_ == double_node.token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << token_.GetValue();
  }

  real_talk::lexer::TokenInfo token_;
};
}
}

#endif
