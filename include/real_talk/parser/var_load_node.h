
#ifndef _REAL_TALK_PARSER_VAR_LOAD_NODE_H_
#define _REAL_TALK_PARSER_VAR_LOAD_NODE_H_

#include <string>
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class VarLoadNode: public ExprNode {
 public:
  explicit VarLoadNode(const real_talk::lexer::TokenInfo &name_token)
      : name_token_(name_token) {
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitVarLoad(*this);
  }

  const real_talk::lexer::TokenInfo &GetNameToken() const {
    return name_token_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const VarLoadNode &rhs = static_cast<const VarLoadNode&>(node);
    return name_token_ == rhs.name_token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << name_token_.GetValue();
  }

  real_talk::lexer::TokenInfo name_token_;
};
}
}

#endif
