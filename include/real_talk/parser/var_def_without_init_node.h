
#ifndef _REAL_TALK_PARSER_VAR_DEF_WITHOUT_INIT_NODE_H_
#define _REAL_TALK_PARSER_VAR_DEF_WITHOUT_INIT_NODE_H_

#include <string>
#include <memory>
#include "real_talk/parser/stmt_node.h"

namespace real_talk {
namespace parser {

class ExprNode;
class DataTypeNode;

class VarDefWithoutInitNode: public StmtNode {
 public:
  VarDefWithoutInitNode(
      std::unique_ptr<DataTypeNode> var_data_type,
      const real_talk::lexer::TokenInfo &var_name_token,
      const real_talk::lexer::TokenInfo &end_token)
      : var_data_type_(move(var_data_type)),
        var_name_token_(var_name_token),
        end_token_(end_token) {
    assert(var_data_type_);
  }

  const real_talk::lexer::TokenInfo &GetVarNameToken() const {
    return var_name_token_;
  }

  const std::unique_ptr<DataTypeNode> &GetVarDataType() const {
    return var_data_type_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const VarDefWithoutInitNode &var_def_node =
        static_cast<const VarDefWithoutInitNode&>(node);
    return *var_data_type_ == *(var_def_node.var_data_type_)
        && var_name_token_ == var_def_node.var_name_token_
        && end_token_ == var_def_node.end_token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *var_data_type_ << ' ' << var_name_token_.GetValue()
           << end_token_.GetValue();
  }

  std::unique_ptr<DataTypeNode> var_data_type_;
  real_talk::lexer::TokenInfo var_name_token_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}
#endif
