
#ifndef _REAL_TALK_PARSER_ELSE_IF_NODE_H_
#define _REAL_TALK_PARSER_ELSE_IF_NODE_H_

#include <memory>
#include "real_talk/parser/if_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class ElseIfNode: public Node {
 public:
  ElseIfNode(
      const real_talk::lexer::TokenInfo &start_token,
      std::unique_ptr<IfNode> if_node)
      : start_token_(start_token),
        if_(move(if_node)) {
    assert(if_);
  }

  const std::unique_ptr<IfNode> &GetIf() const {
    return if_;
  }

  virtual void Accept(NodeVisitor&) const override {
    assert(false);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ElseIfNode &rhs = static_cast<const ElseIfNode&>(node);
    return start_token_ == rhs.start_token_
        && *if_ == *(rhs.if_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << start_token_.GetValue() << *if_;
  }

  real_talk::lexer::TokenInfo start_token_;
  std::unique_ptr<IfNode> if_;
};
}
}

#endif
