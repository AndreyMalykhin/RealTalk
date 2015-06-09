
#ifndef _REAL_TALK_PARSER_ELSE_IF_NODE_H_
#define _REAL_TALK_PARSER_ELSE_IF_NODE_H_

#include <iostream>
#include <memory>
#include "real_talk/parser/if_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class ElseIfNode final {
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

  friend bool operator==(const ElseIfNode &lhs, const ElseIfNode &rhs) {
    return lhs.start_token_ == rhs.start_token_
        && *(lhs.if_) == *(rhs.if_);
  }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const ElseIfNode &node) {
    return stream << node.start_token_.GetValue() << ' ' << *(node.if_);
  }

 private:
  real_talk::lexer::TokenInfo start_token_;
  std::unique_ptr<IfNode> if_;
};
}
}

#endif
