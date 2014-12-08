
#ifndef _REAL_TALK_PARSER_VAR_LOAD_NODE_H_
#define _REAL_TALK_PARSER_VAR_LOAD_NODE_H_

#include <string>
#include "real_talk/parser/expr_node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class VarLoadNode: public ExprNode {
 public:
  explicit VarLoadNode(const real_talk::lexer::TokenInfo &var_name_token)
      : var_name_token_(var_name_token) {
  }

  const real_talk::lexer::TokenInfo &GetVarNameToken() const {
    return var_name_token_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const VarLoadNode &var_load_node = static_cast<const VarLoadNode&>(node);
    return var_name_token_ == var_load_node.var_name_token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << var_name_token_.GetValue();
  }

  real_talk::lexer::TokenInfo var_name_token_;
};
}
}

#endif
