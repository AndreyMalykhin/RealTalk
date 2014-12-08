
#ifndef _REAL_TALK_PARSER_ARRAY_DATA_TYPE_NODE_H_
#define _REAL_TALK_PARSER_ARRAY_DATA_TYPE_NODE_H_

#include <string>
#include "real_talk/parser/data_type_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class ArrayDataTypeNode: public DataTypeNode {
 public:
  ArrayDataTypeNode(const real_talk::lexer::TokenInfo &name_token,
                    const real_talk::lexer::TokenInfo &subscript_start_token,
                    const real_talk::lexer::TokenInfo &subscript_end_token)
      : name_token_(name_token),
        subscript_start_token_(subscript_start_token),
        subscript_end_token_(subscript_end_token) {
  }

  const std::string &GetName() const {
    return name_token_.GetValue();
  }

  const real_talk::lexer::TokenInfo &GetNameToken() const {
    return name_token_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ArrayDataTypeNode &data_type_node =
        static_cast<const ArrayDataTypeNode&>(node);
    return name_token_ == data_type_node.name_token_
        && subscript_start_token_ == data_type_node.subscript_start_token_
        && subscript_end_token_ == data_type_node.subscript_end_token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << name_token_.GetValue() << subscript_start_token_.GetValue()
           << subscript_end_token_.GetValue();
  }

 private:
  real_talk::lexer::TokenInfo name_token_;
  real_talk::lexer::TokenInfo subscript_start_token_;
  real_talk::lexer::TokenInfo subscript_end_token_;
};
}
}
#endif
