
#ifndef _REAL_TALK_PARSER_DOUBLE_DATA_TYPE_NODE_H_
#define _REAL_TALK_PARSER_DOUBLE_DATA_TYPE_NODE_H_

#include "real_talk/parser/primitive_data_type_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class DoubleDataTypeNode: public PrimitiveDataTypeNode {
 public:
  explicit DoubleDataTypeNode(const real_talk::lexer::TokenInfo &name_token)
      : name_token_(name_token) {
  }

  virtual const real_talk::lexer::TokenInfo &GetStartToken() const override {
    return name_token_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitDoubleDataType(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const DoubleDataTypeNode &rhs =
        static_cast<const DoubleDataTypeNode&>(node);
    return name_token_ == rhs.name_token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << name_token_.GetValue();
  }

 private:
  real_talk::lexer::TokenInfo name_token_;
};
}
}
#endif
