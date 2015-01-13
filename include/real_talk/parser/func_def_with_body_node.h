
#ifndef _REAL_TALK_PARSER_FUNC_DEF_WITH_BODY_NODE_H_
#define _REAL_TALK_PARSER_FUNC_DEF_WITH_BODY_NODE_H_

#include <string>
#include <memory>
#include <vector>
#include "real_talk/parser/func_def_node.h"
#include "real_talk/parser/base_func_def_node.h"
#include "real_talk/parser/scope_node.h"

namespace real_talk {
namespace parser {

class FuncDefWithBodyNode: public FuncDefNode {
 public:
  FuncDefWithBodyNode(
      const std::vector<real_talk::lexer::TokenInfo> &modifier_tokens,
      std::unique_ptr<DataTypeNode> return_data_type,
      const real_talk::lexer::TokenInfo &func_name_token,
      const real_talk::lexer::TokenInfo &args_start_token,
      std::vector< std::unique_ptr<ArgDefNode> > args,
      const std::vector<real_talk::lexer::TokenInfo> &arg_separator_tokens,
      const real_talk::lexer::TokenInfo &args_end_token,
      std::unique_ptr<ScopeNode> body)
      : base_func_def_(modifier_tokens,
                       move(return_data_type),
                       func_name_token,
                       args_start_token,
                       move(args),
                       arg_separator_tokens,
                       args_end_token),
        body_(move(body)) {
    assert(body_);
  }

  virtual const std::vector<real_talk::lexer::TokenInfo> &GetModifierTokens()
      const override {
    return base_func_def_.GetModifierTokens();
  }

  const std::unique_ptr<ScopeNode> &GetBody() const {
    return body_;
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
    visitor.VisitFuncDefWithBody(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    return DoIsEqual(static_cast<const FuncDefWithBodyNode&>(node));
  }

  virtual bool IsEqual(const DefNode &node) const override {
    return DoIsEqual(static_cast<const FuncDefWithBodyNode&>(node));
  }

  bool DoIsEqual(const FuncDefWithBodyNode &rhs) const {
    return base_func_def_ == rhs.base_func_def_
        && *body_ == *(rhs.body_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << base_func_def_ << ' ' << *body_;
  }

  BaseFuncDefNode base_func_def_;
  std::unique_ptr<ScopeNode> body_;
};
}
}
#endif
