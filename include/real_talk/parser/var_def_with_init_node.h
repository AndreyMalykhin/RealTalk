
#ifndef _REAL_TALK_PARSER_VAR_DEF_WITH_INIT_NODE_H_
#define _REAL_TALK_PARSER_VAR_DEF_WITH_INIT_NODE_H_

#include <cassert>
#include <string>
#include <memory>
#include "real_talk/lexer/token_info.h"
#include "real_talk/parser/expr_node.h"
#include "real_talk/parser/var_def_node.h"
#include "real_talk/parser/data_type_node.h"
#include "real_talk/parser/node_visitor.h"

namespace real_talk {
namespace parser {

class VarDefWithInitNode: public VarDefNode {
 public:
  VarDefWithInitNode(
      std::unique_ptr<DataTypeNode> data_type,
      const real_talk::lexer::TokenInfo &name_token,
      const real_talk::lexer::TokenInfo &assign_token,
      std::unique_ptr<ExprNode> value,
      const real_talk::lexer::TokenInfo &end_token)
      : data_type_(move(data_type)),
        name_token_(name_token),
        assign_token_(assign_token),
        value_(move(value)),
        end_token_(end_token) {
    assert(data_type_);
    assert(value_);
  }

  virtual const real_talk::lexer::TokenInfo &GetStartToken() const override {
    return data_type_->GetStartToken();
  }

  const std::unique_ptr<ExprNode> &GetValue() const {
    return value_;
  }

  const real_talk::lexer::TokenInfo &GetNameToken() const override {
    return name_token_;
  }

  const std::unique_ptr<DataTypeNode> &GetDataType() const override {
    return data_type_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitVarDefWithInit(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const VarDefWithInitNode &rhs =
        static_cast<const VarDefWithInitNode&>(node);
    return *data_type_ == *(rhs.data_type_)
        && name_token_ == rhs.name_token_
        && assign_token_ == rhs.assign_token_
        && end_token_ == rhs.end_token_
        && *value_ == *(rhs.value_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *data_type_ << ' ' << name_token_.GetValue() << ' '
           << assign_token_.GetValue() << ' ' << *value_
           << end_token_.GetValue();
  }

  std::unique_ptr<DataTypeNode> data_type_;
  real_talk::lexer::TokenInfo name_token_;
  real_talk::lexer::TokenInfo assign_token_;
  std::unique_ptr<ExprNode> value_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}
#endif
