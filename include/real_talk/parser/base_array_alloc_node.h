
#ifndef _REAL_TALK_PARSER_BASE_ARRAY_ALLOC_NODE_H_
#define _REAL_TALK_PARSER_BASE_ARRAY_ALLOC_NODE_H_

#include <memory>
#include "real_talk/parser/primitive_data_type_node.h"

namespace real_talk {
namespace parser {

class BaseArrayAllocNode final {
 public:
  BaseArrayAllocNode(
      const real_talk::lexer::TokenInfo &op_token,
      std::unique_ptr<PrimitiveDataTypeNode> data_type)
      : op_token_(op_token),
      data_type_(move(data_type)) {
    assert(data_type_);
  }

  const std::unique_ptr<PrimitiveDataTypeNode> &GetDataType() const {
    return data_type_;
  }

  friend bool operator==(const BaseArrayAllocNode &lhs,
                         const BaseArrayAllocNode &rhs) {
    return lhs.op_token_ == rhs.op_token_
        && *(lhs.data_type_) == *(rhs.data_type_);
  }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const BaseArrayAllocNode &node) {
    return stream << node.op_token_.GetValue() << ' ' << *(node.data_type_);
  }

 private:
  real_talk::lexer::TokenInfo op_token_;
  std::unique_ptr<PrimitiveDataTypeNode> data_type_;
};
}
}
#endif
