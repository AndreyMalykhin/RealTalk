
#ifndef _REAL_TALK_PARSER_BASE_FUNC_DEF_NODE_H_
#define _REAL_TALK_PARSER_BASE_FUNC_DEF_NODE_H_

#include <boost/iterator/indirect_iterator.hpp>
#include <string>
#include <memory>
#include <vector>
#include "real_talk/parser/arg_def_node.h"
#include "real_talk/parser/data_type_node.h"

namespace real_talk {
namespace parser {

class BaseFuncDefNode final {
 public:
  BaseFuncDefNode(
      const std::vector<real_talk::lexer::TokenInfo> &modifier_tokens,
      std::unique_ptr<DataTypeNode> return_data_type,
      const real_talk::lexer::TokenInfo &func_name_token,
      const real_talk::lexer::TokenInfo &args_start_token,
      std::vector< std::unique_ptr<ArgDefNode> > args,
      const std::vector<real_talk::lexer::TokenInfo> &arg_separator_tokens,
      const real_talk::lexer::TokenInfo &args_end_token)
      : modifier_tokens_(modifier_tokens),
        return_data_type_(move(return_data_type)),
        func_name_token_(func_name_token),
        args_start_token_(args_start_token),
        args_(move(args)),
        arg_separator_tokens_(arg_separator_tokens),
        args_end_token_(args_end_token) {
    assert(return_data_type_);
    assert((args_.empty() && arg_separator_tokens_.empty())
           || (arg_separator_tokens_.size() == args_.size() - 1));
  }

  const real_talk::lexer::TokenInfo &GetStartToken() const {
    return modifier_tokens_.empty() ? return_data_type_->GetStartToken()
        : modifier_tokens_.front();
  }

  const std::vector<real_talk::lexer::TokenInfo> &GetModifierTokens() const {
    return modifier_tokens_;
  }

  const real_talk::lexer::TokenInfo &GetArgsStartToken() const {
    return args_start_token_;
  }

  const real_talk::lexer::TokenInfo &GetArgsEndToken() const {
    return args_end_token_;
  }

  const std::vector< std::unique_ptr<ArgDefNode> > &GetArgs() const {
    return args_;
  }

  const std::vector<real_talk::lexer::TokenInfo> &GetArgSeparatorTokens()
      const {
    return arg_separator_tokens_;
  }

  const real_talk::lexer::TokenInfo &GetNameToken() const {
    return func_name_token_;
  }

  const std::unique_ptr<DataTypeNode> &GetDataType() const {
    return return_data_type_;
  }

  friend bool operator==(const BaseFuncDefNode &lhs,
                         const BaseFuncDefNode &rhs) {
    return *(lhs.return_data_type_) == *(rhs.return_data_type_)
        && lhs.func_name_token_ == rhs.func_name_token_
        && lhs.args_start_token_ == rhs.args_start_token_
        && lhs.args_end_token_ == rhs.args_end_token_
        && lhs.args_.size() == rhs.args_.size()
        && lhs.arg_separator_tokens_.size() ==
        rhs.arg_separator_tokens_.size()
        && lhs.modifier_tokens_.size() == rhs.modifier_tokens_.size()
        && std::equal(lhs.modifier_tokens_.begin(),
                      lhs.modifier_tokens_.end(),
                      rhs.modifier_tokens_.begin())
        && std::equal(lhs.arg_separator_tokens_.begin(),
                      lhs.arg_separator_tokens_.end(),
                      rhs.arg_separator_tokens_.begin())
        && std::equal(boost::make_indirect_iterator(lhs.args_.begin()),
                      boost::make_indirect_iterator(lhs.args_.end()),
                      boost::make_indirect_iterator(rhs.args_.begin()));
  }

  friend std::ostream &operator<<(std::ostream &stream,
                                 const BaseFuncDefNode &node) {
    for (const real_talk::lexer::TokenInfo &modifier_token:
             node.modifier_tokens_) {
      stream << modifier_token.GetValue() << ' ';
    }

    stream << *(node.return_data_type_) << ' '
           << node.func_name_token_.GetValue()
           << node.args_start_token_.GetValue();

    if (!node.args_.empty()) {
      auto last_arg_it = node.args_.end() - 1;
      auto arg_separator_token_it = node.arg_separator_tokens_.begin();

      for (auto arg_it = node.args_.begin(); arg_it != last_arg_it; ++arg_it) {
        stream << **arg_it << arg_separator_token_it->GetValue() << ' ';
        ++arg_separator_token_it;
      }

      stream << **last_arg_it;
    }

    return stream << node.args_end_token_.GetValue();
  }

 private:
  std::vector<real_talk::lexer::TokenInfo> modifier_tokens_;
  std::unique_ptr<DataTypeNode> return_data_type_;
  real_talk::lexer::TokenInfo func_name_token_;
  real_talk::lexer::TokenInfo args_start_token_;
  std::vector< std::unique_ptr<ArgDefNode> > args_;
  std::vector<real_talk::lexer::TokenInfo> arg_separator_tokens_;
  real_talk::lexer::TokenInfo args_end_token_;
};
}
}
#endif
