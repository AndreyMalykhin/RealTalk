
#ifndef _REAL_TALK_PARSER_CONTINUE_NODE_H_
#define _REAL_TALK_PARSER_CONTINUE_NODE_H_

#include <memory>
#include "real_talk/parser/control_flow_transfer_node.h"

namespace real_talk {
namespace parser {

class ContinueNode: public ControlFlowTransferNode {
 public:
  ContinueNode(
      const real_talk::lexer::TokenInfo &start_token,
      const real_talk::lexer::TokenInfo &end_token)
      : start_token_(start_token),
        end_token_(end_token) {
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitContinue(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ContinueNode &rhs = static_cast<const ContinueNode&>(node);
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
