
#ifndef _REAL_TALK_PARSER_INT_DATA_TYPE_NODE_H_
#define _REAL_TALK_PARSER_INT_DATA_TYPE_NODE_H_

#include <string>
#include "real_talk/parser/data_type_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class IntDataTypeNode: public DataTypeNode {
 public:
  explicit IntDataTypeNode(const real_talk::lexer::TokenInfo &name_token)
      : name_token_(name_token) {
  }

  const real_talk::lexer::TokenInfo &GetNameToken() const {
    return name_token_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitIntDataType(*this);
  }

  virtual std::unique_ptr<DataTypeNode> Clone() const override {
    return std::unique_ptr<DataTypeNode>(new IntDataTypeNode(name_token_));
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
