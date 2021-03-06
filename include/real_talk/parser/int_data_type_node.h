
#ifndef _REAL_TALK_PARSER_INT_DATA_TYPE_NODE_H_
#define _REAL_TALK_PARSER_INT_DATA_TYPE_NODE_H_

#include "real_talk/parser/primitive_data_type_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class IntDataTypeNode: public PrimitiveDataTypeNode {
 public:
  explicit IntDataTypeNode(const real_talk::lexer::TokenInfo &name_token)
      : name_token_(name_token) {
  }

  virtual const real_talk::lexer::TokenInfo &GetStartToken() const override {
    return name_token_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitIntDataType(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const IntDataTypeNode &rhs =
        static_cast<const IntDataTypeNode&>(node);
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
