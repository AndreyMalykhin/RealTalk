
#ifndef _REAL_TALK_PARSER_IF_ELSE_IF_ELSE_NODE_H_
#define _REAL_TALK_PARSER_IF_ELSE_IF_ELSE_NODE_H_

#include <vector>
#include "real_talk/parser/if_else_if_node.h"

namespace real_talk {
namespace parser {

class IfElseIfElseNode: public BranchNode {
 public:
  IfElseIfElseNode(
      std::unique_ptr<IfNode> if_node,
      std::vector< std::unique_ptr<ElseIfNode> > else_if_nodes,
      const real_talk::lexer::TokenInfo &else_token,
      std::unique_ptr<ScopeNode> else_body)
      : if_else_if_(move(if_node), move(else_if_nodes)),
        else_token_(else_token),
        else_body_(move(else_body)) {
    assert(else_body_);
  }

  const std::unique_ptr<IfNode> &GetIf() const override {
    return if_else_if_.GetIf();
  }

  const std::vector< std::unique_ptr<ElseIfNode> > &GetElseIfs()
      const override {
    return if_else_if_.GetElseIfs();
  }

  const std::unique_ptr<ScopeNode> &GetElseBody() const {
    return else_body_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitIfElseIfElse(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const IfElseIfElseNode &rhs = static_cast<const IfElseIfElseNode&>(node);
    return if_else_if_ == rhs.if_else_if_
        && else_token_ == rhs.else_token_
        && *else_body_ == *(rhs.else_body_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << if_else_if_ << ' ' << else_token_.GetValue() << ' ' << *else_body_;
  }

  IfElseIfNode if_else_if_;
  const real_talk::lexer::TokenInfo else_token_;
  std::unique_ptr<ScopeNode> else_body_;
};
}
}

#endif
