
#ifndef _REAL_TALK_PARSER_FUNC_DEF_NODE_H_
#define _REAL_TALK_PARSER_FUNC_DEF_NODE_H_

#include <boost/iterator/indirect_iterator.hpp>
#include <string>
#include <memory>
#include <vector>
#include "real_talk/parser/stmt_node.h"
#include "real_talk/parser/scope_node.h"
#include "real_talk/parser/arg_def_node.h"

namespace real_talk {
namespace parser {

class DataTypeNode;

class FuncDefNode: public StmtNode {
 public:
  FuncDefNode(
      std::unique_ptr<DataTypeNode> return_data_type,
      const real_talk::lexer::TokenInfo &func_name_token,
      const real_talk::lexer::TokenInfo &args_start_token,
      std::vector< std::unique_ptr<ArgDefNode> > args,
      const std::vector<real_talk::lexer::TokenInfo> &arg_separator_tokens,
      const real_talk::lexer::TokenInfo &args_end_token,
      std::unique_ptr<ScopeNode> body)
      : return_data_type_(move(return_data_type)),
        func_name_token_(func_name_token),
        args_start_token_(args_start_token),
        args_(move(args)),
        arg_separator_tokens_(arg_separator_tokens),
        args_end_token_(args_end_token),
        body_(move(body)) {
    assert(return_data_type_);
    assert(body_);
    assert((args_.empty() && arg_separator_tokens_.empty())
           || (arg_separator_tokens_.size() == args_.size() - 1));
  }

  const real_talk::lexer::TokenInfo &GetFuncNameToken() const {
    return func_name_token_;
  }

  const std::string &GetFuncName() const {
    return func_name_token_.GetValue();
  }

  const std::unique_ptr<DataTypeNode> &GetReturnDataType() const {
    return return_data_type_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitFuncDef(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const FuncDefNode &rhs = static_cast<const FuncDefNode&>(node);
    return *return_data_type_ == *(rhs.return_data_type_)
        && func_name_token_ == rhs.func_name_token_
        && args_start_token_ == rhs.args_start_token_
        && args_end_token_ == rhs.args_end_token_
        && args_.size() == rhs.args_.size()
        && arg_separator_tokens_.size() ==
        rhs.arg_separator_tokens_.size()
        && std::equal(
            arg_separator_tokens_.begin(),
            arg_separator_tokens_.end(),
            rhs.arg_separator_tokens_.begin())
        && std::equal(
            boost::make_indirect_iterator(args_.begin()),
            boost::make_indirect_iterator(args_.end()),
            boost::make_indirect_iterator(rhs.args_.begin()))
        && *body_ == *(rhs.body_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *return_data_type_ << ' ' << func_name_token_.GetValue()
           << args_start_token_.GetValue();

    if (!args_.empty()) {
      auto last_arg_it = args_.end() - 1;
      auto arg_separator_token_it = arg_separator_tokens_.begin();

      for (auto arg_it = args_.begin(); arg_it != last_arg_it; ++arg_it) {
        stream << **arg_it << arg_separator_token_it->GetValue() << ' ';
        ++arg_separator_token_it;
      }

      stream << **last_arg_it;
    }

    stream << args_end_token_.GetValue() << ' ' << *body_;
  }

  std::unique_ptr<DataTypeNode> return_data_type_;
  real_talk::lexer::TokenInfo func_name_token_;
  real_talk::lexer::TokenInfo args_start_token_;
  std::vector< std::unique_ptr<ArgDefNode> > args_;
  std::vector<real_talk::lexer::TokenInfo> arg_separator_tokens_;
  real_talk::lexer::TokenInfo args_end_token_;
  std::unique_ptr<ScopeNode> body_;
};
}
}
#endif
