
#ifndef _REAL_TALK_PARSER_VAR_DEF_WITH_INIT_NODE_H_
#define _REAL_TALK_PARSER_VAR_DEF_WITH_INIT_NODE_H_

#include <string>
#include <memory>
#include "real_talk/parser/stmt_node.h"
#include "real_talk/parser/data_type_node.h"

namespace real_talk {
namespace parser {

class ExprNode;

class VarDefWithInitNode: public StmtNode {
 public:
  VarDefWithInitNode(
      std::unique_ptr<DataTypeNode> var_data_type,
      const real_talk::lexer::TokenInfo &var_name_token,
      const real_talk::lexer::TokenInfo &assign_token,
      std::unique_ptr<ExprNode> var_value,
      const real_talk::lexer::TokenInfo &end_token)
      : var_data_type_(move(var_data_type)),
        var_name_token_(var_name_token),
        assign_token_(assign_token),
        var_value_(move(var_value)),
        end_token_(end_token) {
    assert(var_data_type_);
    assert(var_value_);
  }

  const real_talk::lexer::TokenInfo &GetVarNameToken() const {
    return var_name_token_;
  }

  const std::unique_ptr<ExprNode> &GetVarValue() const {
    return var_value_;
  }

  const std::string &GetVarName() const {
    return var_name_token_.GetValue();
  }

  const std::unique_ptr<DataTypeNode> &GetVarDataType() const {
    return var_data_type_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const VarDefWithInitNode &var_def_node =
        static_cast<const VarDefWithInitNode&>(node);
    return *var_data_type_ == *(var_def_node.var_data_type_)
        && var_name_token_ == var_def_node.var_name_token_
        && assign_token_ == var_def_node.assign_token_
        && end_token_ == var_def_node.end_token_
        && *var_value_ == *(var_def_node.var_value_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *var_data_type_ << ' ' << var_name_token_.GetValue() << ' '
           << assign_token_.GetValue() << ' ' << *var_value_
           << end_token_.GetValue();
  }

  std::unique_ptr<DataTypeNode> var_data_type_;
  real_talk::lexer::TokenInfo var_name_token_;
  real_talk::lexer::TokenInfo assign_token_;
  std::unique_ptr<ExprNode> var_value_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}
#endif
