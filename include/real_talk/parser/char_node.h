
#ifndef _REAL_TALK_PARSER_CHAR_NODE_H_
#define _REAL_TALK_PARSER_CHAR_NODE_H_

#include <string>
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class CharNode: public ExprNode {
 public:
  explicit CharNode(const real_talk::lexer::TokenInfo &token): token_(token) {
  }

  const real_talk::lexer::TokenInfo &GetToken() const {
    return token_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const CharNode &char_node = static_cast<const CharNode&>(node);
    return token_ == char_node.token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << token_.GetValue();
  }

  real_talk::lexer::TokenInfo token_;
};
}
}

#endif
