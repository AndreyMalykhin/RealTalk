
#ifndef _REAL_TALK_PARSER_ARRAY_ALLOC_NODE_H_
#define _REAL_TALK_PARSER_ARRAY_ALLOC_NODE_H_

#include <cstdint>
#include <vector>
#include <memory>
#include "real_talk/parser/expr_node.h"
#include "real_talk/parser/bounded_array_data_type_node.h"

namespace real_talk {
namespace parser {

class ArrayAllocNode final: public ExprNode {
 public:
  ArrayAllocNode(
      const real_talk::lexer::TokenInfo &op_token,
      std::unique_ptr<BoundedArrayDataTypeNode> data_type)
      : op_token_(op_token),
      data_type_(move(data_type)) {
    assert(data_type_);
  }

  const std::unique_ptr<BoundedArrayDataTypeNode> &GetDataType() const {
    return data_type_;
  }

  virtual void Accept(NodeVisitor&) const override {
    assert(false);
  }

  virtual bool IsEqual(const Node &node) const override {
    const ArrayAllocNode &alloc_node =
        static_cast<const ArrayAllocNode&>(node);
    return op_token_ == alloc_node.op_token_
        && *data_type_ == *(alloc_node.data_type_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << op_token_.GetValue() << ' ' << *data_type_;
  }

 private:
  real_talk::lexer::TokenInfo op_token_;
  std::unique_ptr<BoundedArrayDataTypeNode> data_type_;
};
}
}
#endif
