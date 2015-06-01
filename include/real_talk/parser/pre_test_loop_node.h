
#ifndef _REAL_TALK_PARSER_PRE_TEST_LOOP_NODE_H_
#define _REAL_TALK_PARSER_PRE_TEST_LOOP_NODE_H_

#include <cassert>
#include <memory>
#include "real_talk/parser/stmt_node.h"
#include "real_talk/parser/scope_node.h"
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class PreTestLoopNode: public StmtNode {
 public:
  PreTestLoopNode(
      const real_talk::lexer::TokenInfo &start_token,
      const real_talk::lexer::TokenInfo &cond_start_token,
      std::unique_ptr<ExprNode> cond,
      const real_talk::lexer::TokenInfo &cond_end_token,
      std::unique_ptr<ScopeNode> body)
      : start_token_(start_token),
        cond_start_token_(cond_start_token),
        cond_(move(cond)),
        cond_end_token_(cond_end_token),
        body_(move(body)) {
    assert(cond_);
    assert(body_);
  }

  virtual const real_talk::lexer::TokenInfo &GetStartToken() const override {
    return start_token_;
  }

  const std::unique_ptr<ExprNode> &GetCond() const {
    return cond_;
  }

  const std::unique_ptr<ScopeNode> &GetBody() const {
    return body_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitPreTestLoop(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const PreTestLoopNode &rhs = static_cast<const PreTestLoopNode&>(node);
    return start_token_ == rhs.start_token_
        && cond_start_token_ == rhs.cond_start_token_
        && cond_end_token_ == rhs.cond_end_token_
        && *cond_ == *(rhs.cond_)
        && *body_ == *(rhs.body_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << start_token_.GetValue() << ' ' << cond_start_token_.GetValue()
           << *cond_ << cond_end_token_.GetValue() << ' ' << *body_;
  }

  real_talk::lexer::TokenInfo start_token_;
  real_talk::lexer::TokenInfo cond_start_token_;
  std::unique_ptr<ExprNode> cond_;
  real_talk::lexer::TokenInfo cond_end_token_;
  std::unique_ptr<ScopeNode> body_;
};
}
}

#endif
