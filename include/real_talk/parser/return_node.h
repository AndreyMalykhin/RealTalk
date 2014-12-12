
#ifndef _REAL_TALK_PARSER_RETURN_NODE_H_
#define _REAL_TALK_PARSER_RETURN_NODE_H_

#include <memory>
#include "real_talk/parser/stmt_node.h"

namespace real_talk {
namespace parser {

class ReturnNode: public StmtNode {
 public:
  ReturnNode(
      const real_talk::lexer::TokenInfo &start_token,
      const real_talk::lexer::TokenInfo &end_token)
      : start_token_(start_token),
        end_token_(end_token) {
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitReturn(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ReturnNode &rhs = static_cast<const ReturnNode&>(node);
    return start_token_ == rhs.start_token_
        && end_token_ == rhs.end_token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << start_token_.GetValue() << end_token_.GetValue();
  }

  real_talk::lexer::TokenInfo start_token_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}
#endif
