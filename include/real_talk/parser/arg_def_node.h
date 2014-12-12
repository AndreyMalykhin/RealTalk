
#ifndef _REAL_TALK_PARSER_ARG_DEF_NODE_H_
#define _REAL_TALK_PARSER_ARG_DEF_NODE_H_

#include <string>
#include <memory>
#include "real_talk/parser/node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class DataTypeNode;

class ArgDefNode: public Node {
 public:
  ArgDefNode(
      std::unique_ptr<DataTypeNode> arg_data_type,
      const real_talk::lexer::TokenInfo &arg_name_token)
      : arg_data_type_(move(arg_data_type)),
        arg_name_token_(arg_name_token) {
    assert(arg_data_type_);
  }

  const std::string &GetArgName() const {
    return arg_name_token_.GetValue();
  }

  const std::unique_ptr<DataTypeNode> &GetArgDataType() const {
    return arg_data_type_;
  }

  const real_talk::lexer::TokenInfo &GetArgNameToken() const {
    return arg_name_token_;
  }

  virtual void Accept(NodeVisitor&) const override {
    assert(false);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ArgDefNode &arg_def_node = static_cast<const ArgDefNode&>(node);
    return *arg_data_type_ == *(arg_def_node.arg_data_type_)
        && arg_name_token_ == arg_def_node.arg_name_token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *arg_data_type_ << ' ' << arg_name_token_.GetValue();
  }

  std::unique_ptr<DataTypeNode> arg_data_type_;
  real_talk::lexer::TokenInfo arg_name_token_;
};
}
}
#endif
