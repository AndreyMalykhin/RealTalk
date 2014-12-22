
#ifndef _REAL_TALK_PARSER_BOUNDED_ARRAY_DATA_TYPE_NODE_H_
#define _REAL_TALK_PARSER_BOUNDED_ARRAY_DATA_TYPE_NODE_H_

#include <string>
#include "real_talk/parser/data_type_node.h"
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class BoundedArrayDataTypeNode: public DataTypeNode {
 public:
  BoundedArrayDataTypeNode(
      std::unique_ptr<DataTypeNode> element_data_type,
      const real_talk::lexer::TokenInfo &subscript_start_token,
      std::unique_ptr<ExprNode> size,
      const real_talk::lexer::TokenInfo &subscript_end_token)
      : element_data_type_(move(element_data_type)),
        subscript_start_token_(subscript_start_token),
        size_(move(size)),
        subscript_end_token_(subscript_end_token) {
    assert(element_data_type_);
    assert(size_);
  }

  const std::unique_ptr<ExprNode> &GetSize() const {
    return size_;
  }

  const std::unique_ptr<DataTypeNode> &GetElementDataType() const {
    return element_data_type_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitBoundedArrayDataType(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const BoundedArrayDataTypeNode &rhs =
        static_cast<const BoundedArrayDataTypeNode&>(node);
    return subscript_start_token_ == rhs.subscript_start_token_
        && subscript_end_token_ == rhs.subscript_end_token_
        && *size_ == *(rhs.size_)
        && *element_data_type_ == *(rhs.element_data_type_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *element_data_type_ << subscript_start_token_.GetValue() << *size_
           << subscript_end_token_.GetValue();
  }

 private:
  std::unique_ptr<DataTypeNode> element_data_type_;
  real_talk::lexer::TokenInfo subscript_start_token_;
  std::unique_ptr<ExprNode> size_;
  real_talk::lexer::TokenInfo subscript_end_token_;
};
}
}
#endif
