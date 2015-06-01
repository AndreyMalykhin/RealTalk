
#ifndef _REAL_TALK_PARSER_ARRAY_DATA_TYPE_NODE_H_
#define _REAL_TALK_PARSER_ARRAY_DATA_TYPE_NODE_H_

#include <cassert>
#include "real_talk/parser/data_type_node.h"
#include "real_talk/parser/node_visitor.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class ArrayDataTypeNode: public DataTypeNode {
 public:
  ArrayDataTypeNode(
      std::unique_ptr<DataTypeNode> element_data_type,
      const real_talk::lexer::TokenInfo &subscript_start_token,
      const real_talk::lexer::TokenInfo &subscript_end_token)
      : element_data_type_(move(element_data_type)),
        subscript_start_token_(subscript_start_token),
        subscript_end_token_(subscript_end_token) {
    assert(element_data_type_);
  }

  virtual const real_talk::lexer::TokenInfo &GetStartToken() const override {
    return element_data_type_->GetStartToken();
  }

  const std::unique_ptr<DataTypeNode> &GetElementDataType() const {
    return element_data_type_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitArrayDataType(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ArrayDataTypeNode &rhs = static_cast<const ArrayDataTypeNode&>(node);
    return subscript_start_token_ == rhs.subscript_start_token_
        && subscript_end_token_ == rhs.subscript_end_token_
        && *element_data_type_ == *(rhs.element_data_type_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *element_data_type_ << subscript_start_token_.GetValue()
           << subscript_end_token_.GetValue();
  }

 private:
  std::unique_ptr<DataTypeNode> element_data_type_;
  real_talk::lexer::TokenInfo subscript_start_token_;
  real_talk::lexer::TokenInfo subscript_end_token_;
};
}
}
#endif
