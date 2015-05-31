
#ifndef _REAL_TALK_PARSER_ID_NODE_H_
#define _REAL_TALK_PARSER_ID_NODE_H_

#include <string>
#include "real_talk/parser/expr_node.h"
#include "real_talk/parser/node_visitor.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class IdNode: public ExprNode {
 public:
  explicit IdNode(const real_talk::lexer::TokenInfo &name_token)
      : name_token_(name_token) {}

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitId(*this);
  }

  virtual const real_talk::lexer::TokenInfo &GetStartToken() const override {
    return name_token_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const IdNode &rhs = static_cast<const IdNode&>(node);
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
