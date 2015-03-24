
#ifndef _REAL_TALK_PARSER_IF_NODE_H_
#define _REAL_TALK_PARSER_IF_NODE_H_

#include <memory>
#include "real_talk/parser/scope_node.h"
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class IfNode final {
 public:
  IfNode(
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

  const std::unique_ptr<ScopeNode> &GetBody() const {
    return body_;
  }

  const std::unique_ptr<ExprNode> &GetCond() const {
    return cond_;
  }

  friend bool operator==(const IfNode &lhs, const IfNode &rhs) {
    return lhs.start_token_ == rhs.start_token_
        && lhs.cond_start_token_ == rhs.cond_start_token_
        && lhs.cond_end_token_ == rhs.cond_end_token_
        && *(lhs.cond_) == *(rhs.cond_)
        && *(lhs.body_) == *(rhs.body_);
  }

  friend std::ostream &operator<<(std::ostream &stream, const IfNode &node) {
    return stream << node.start_token_.GetValue() << ' '
                  << node.cond_start_token_.GetValue() << *(node.cond_)
                  << node.cond_end_token_.GetValue() << ' ' << *(node.body_);
  }

 private:
  real_talk::lexer::TokenInfo start_token_;
  real_talk::lexer::TokenInfo cond_start_token_;
  std::unique_ptr<ExprNode> cond_;
  real_talk::lexer::TokenInfo cond_end_token_;
  std::unique_ptr<ScopeNode> body_;
};
}
}

#endif
