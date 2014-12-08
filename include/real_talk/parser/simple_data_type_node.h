
#ifndef _REAL_TALK_PARSER_SIMPLE_DATA_TYPE_NODE_H_
#define _REAL_TALK_PARSER_SIMPLE_DATA_TYPE_NODE_H_

#include <string>
#include "real_talk/parser/data_type_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class SimpleDataTypeNode: public DataTypeNode {
 public:
  explicit SimpleDataTypeNode(const real_talk::lexer::TokenInfo &name_token)
      : name_token_(name_token) {
  }

  const std::string &GetName() const {
    return name_token_.GetValue();
  }

  const real_talk::lexer::TokenInfo &GetNameToken() const {
    return name_token_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const SimpleDataTypeNode &data_type_node =
        static_cast<const SimpleDataTypeNode&>(node);
    return name_token_ == data_type_node.name_token_;
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
