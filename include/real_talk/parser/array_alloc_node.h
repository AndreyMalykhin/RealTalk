
#ifndef _REAL_TALK_PARSER_ARRAY_ALLOC_NODE_H_
#define _REAL_TALK_PARSER_ARRAY_ALLOC_NODE_H_

#include <cstdint>
#include <vector>
#include <memory>
#include "real_talk/parser/expr_node.h"
#include "real_talk/parser/simple_data_type_node.h"

namespace real_talk {
namespace parser {

class ArrayAllocNode final: public ExprNode {
 public:
  ArrayAllocNode(
      const real_talk::lexer::TokenInfo &op_token,
      std::unique_ptr<SimpleDataTypeNode> data_type,
      const real_talk::lexer::TokenInfo &subscript_start_token,
      std::unique_ptr<ExprNode> size,
      const real_talk::lexer::TokenInfo &subscript_end_token)
      : op_token_(op_token),
      data_type_(move(data_type)),
      subscript_start_token_(subscript_start_token),
      size_(move(size)),
      subscript_end_token_(subscript_end_token) {
        assert(data_type_);
        assert(size_);
  }

  virtual void Accept(NodeVisitor&) const override {
    assert(false);
  }

  virtual bool IsEqual(const Node &node) const override {
    const ArrayAllocNode &alloc_node =
        static_cast<const ArrayAllocNode&>(node);
    return op_token_ == alloc_node.op_token_
        && *data_type_ == *(alloc_node.data_type_)
        && subscript_start_token_ == alloc_node.subscript_start_token_
        && subscript_end_token_ == alloc_node.subscript_end_token_
        && *size_ == *(alloc_node.size_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << op_token_.GetValue() << ' ' << *data_type_
           << subscript_start_token_.GetValue() << *size_
           << subscript_end_token_.GetValue();
  }

 private:
  real_talk::lexer::TokenInfo op_token_;
  std::unique_ptr<SimpleDataTypeNode> data_type_;
  real_talk::lexer::TokenInfo subscript_start_token_;
  std::unique_ptr<ExprNode> size_;
  real_talk::lexer::TokenInfo subscript_end_token_;
};
}
}
#endif
