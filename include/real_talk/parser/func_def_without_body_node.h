
#ifndef _REAL_TALK_PARSER_FUNC_DEF_WITHOUT_BODY_NODE_H_
#define _REAL_TALK_PARSER_FUNC_DEF_WITHOUT_BODY_NODE_H_

#include <string>
#include <memory>
#include <vector>
#include "real_talk/parser/func_def_node.h"
#include "real_talk/parser/base_func_def_node.h"

namespace real_talk {
namespace parser {

class FuncDefWithoutBodyNode: public FuncDefNode {
 public:
  FuncDefWithoutBodyNode(
      const std::vector<real_talk::lexer::TokenInfo> &modifier_tokens,
      std::unique_ptr<DataTypeNode> return_data_type,
      const real_talk::lexer::TokenInfo &func_name_token,
      const real_talk::lexer::TokenInfo &args_start_token,
      std::vector< std::unique_ptr<ArgDefNode> > args,
      const std::vector<real_talk::lexer::TokenInfo> &arg_separator_tokens,
      const real_talk::lexer::TokenInfo &args_end_token,
      const real_talk::lexer::TokenInfo &end_token)
      : base_func_def_(modifier_tokens,
                       move(return_data_type),
                       func_name_token,
                       args_start_token,
                       move(args),
                       arg_separator_tokens,
                       args_end_token),
        end_token_(end_token) {
  }

  virtual const std::vector<real_talk::lexer::TokenInfo> &GetModifierTokens()
      const override {
    return base_func_def_.GetModifierTokens();
  }

  const real_talk::lexer::TokenInfo &GetEndToken() const {
    return end_token_;
  }

  virtual const real_talk::lexer::TokenInfo &GetArgsStartToken()
      const override {
    return base_func_def_.GetArgsStartToken();
  }

  virtual const real_talk::lexer::TokenInfo &GetArgsEndToken() const override {
    return base_func_def_.GetArgsEndToken();
  }

  virtual const std::vector< std::unique_ptr<ArgDefNode> > &GetArgs()
      const override {
    return base_func_def_.GetArgs();
  }

  virtual const std::vector<real_talk::lexer::TokenInfo>
  &GetArgSeparatorTokens() const override {
    return base_func_def_.GetArgSeparatorTokens();
  }

  virtual const real_talk::lexer::TokenInfo &GetNameToken() const override {
    return base_func_def_.GetNameToken();
  }

  virtual const std::unique_ptr<DataTypeNode> &GetDataType() const override {
    return base_func_def_.GetDataType();
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitFuncDefWithoutBody(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const FuncDefWithoutBodyNode &rhs =
        static_cast<const FuncDefWithoutBodyNode&>(node);
    return base_func_def_ == rhs.base_func_def_
        && end_token_ == rhs.end_token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << base_func_def_ << end_token_.GetValue();
  }

  BaseFuncDefNode base_func_def_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}
#endif
