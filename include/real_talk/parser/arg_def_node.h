
#ifndef _REAL_TALK_PARSER_ARG_DEF_NODE_H_
#define _REAL_TALK_PARSER_ARG_DEF_NODE_H_

#include <string>
#include <memory>
#include "real_talk/parser/node.h"
#include "real_talk/parser/var_def_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class DataTypeNode;

class ArgDefNode: public Node, public VarDefNode {
 public:
  ArgDefNode(
      std::unique_ptr<DataTypeNode> data_type,
      const real_talk::lexer::TokenInfo &name_token)
      : data_type_(move(data_type)),
        name_token_(name_token) {
    assert(data_type_);
  }

  const std::unique_ptr<DataTypeNode> &GetDataType() const override {
    return data_type_;
  }

  const real_talk::lexer::TokenInfo &GetNameToken() const override {
    return name_token_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitArgDef(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    return DoIsEqual(static_cast<const ArgDefNode&>(node));
  }

  virtual bool IsEqual(const DefNode &node) const override {
    return DoIsEqual(static_cast<const ArgDefNode&>(node));
  }

  bool DoIsEqual(const ArgDefNode &rhs) const {
    return *data_type_ == *(rhs.data_type_)
        && name_token_ == rhs.name_token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *data_type_ << ' ' << name_token_.GetValue();
  }

  std::unique_ptr<DataTypeNode> data_type_;
  real_talk::lexer::TokenInfo name_token_;
};
}
}
#endif
