
#ifndef _REAL_TALK_PARSER_UNBOUNDED_ARRAY_SIZE_NODE_H_
#define _REAL_TALK_PARSER_UNBOUNDED_ARRAY_SIZE_NODE_H_

#include <iostream>
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class UnboundedArraySizeNode final {
 public:
  UnboundedArraySizeNode(
      const real_talk::lexer::TokenInfo &start_token,
      const real_talk::lexer::TokenInfo &end_token)
      : start_token_(start_token),
        end_token_(end_token) {
  }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const UnboundedArraySizeNode &node) {
    return stream << node.start_token_.GetValue() << node.end_token_.GetValue();
  }

  friend bool operator==(const UnboundedArraySizeNode &lhs,
                         const UnboundedArraySizeNode &rhs) {
    return lhs.start_token_ == rhs.start_token_
        && lhs.end_token_ == rhs.end_token_;
  }

 private:
  real_talk::lexer::TokenInfo start_token_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}
#endif
