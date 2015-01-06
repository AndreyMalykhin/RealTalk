
#ifndef _REAL_TALK_PARSER_BOUNDED_ARRAY_SIZE_NODE_H_
#define _REAL_TALK_PARSER_BOUNDED_ARRAY_SIZE_NODE_H_

#include <cassert>
#include <memory>
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/expr_node.h"

namespace real_talk {
namespace parser {

class BoundedArraySizeNode final {
 public:
  BoundedArraySizeNode(
      const real_talk::lexer::TokenInfo &start_token,
      std::unique_ptr<ExprNode> value,
      const real_talk::lexer::TokenInfo &end_token)
      : start_token_(start_token),
        value_(move(value)),
        end_token_(end_token) {
    assert(value_);
  }

  const std::unique_ptr<ExprNode> &GetValue() const {
    return value_;
  }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const BoundedArraySizeNode &node) {
    return stream << node.start_token_.GetValue() << *(node.value_)
                  << node.end_token_.GetValue();
  }

  friend bool operator==(const BoundedArraySizeNode &lhs,
                         const BoundedArraySizeNode &rhs) {
    return lhs.start_token_ == rhs.start_token_
        && lhs.end_token_ == rhs.end_token_
        && *(lhs.value_) == *(rhs.value_);
  }

 private:
  real_talk::lexer::TokenInfo start_token_;
  std::unique_ptr<ExprNode> value_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}
#endif
