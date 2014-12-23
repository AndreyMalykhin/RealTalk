
#ifndef _REAL_TALK_PARSER_BOOL_DATA_TYPE_NODE_H_
#define _REAL_TALK_PARSER_BOOL_DATA_TYPE_NODE_H_

#include <string>
#include "real_talk/parser/primitive_data_type_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class BoolDataTypeNode: public PrimitiveDataTypeNode {
 public:
  explicit BoolDataTypeNode(const real_talk::lexer::TokenInfo &name_token)
      : name_token_(name_token) {
  }

  const real_talk::lexer::TokenInfo &GetNameToken() const {
    return name_token_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitBoolDataType(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const BoolDataTypeNode &rhs =
        static_cast<const BoolDataTypeNode&>(node);
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
