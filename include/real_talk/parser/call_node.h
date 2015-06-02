
#ifndef _REAL_TALK_PARSER_CALL_NODE_H_
#define _REAL_TALK_PARSER_CALL_NODE_H_

#include <boost/iterator/indirect_iterator.hpp>
#include <cassert>
#include <vector>
#include <memory>
#include "real_talk/parser/expr_node.h"
#include "real_talk/parser/node_visitor.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class CallNode: public ExprNode {
 public:
  CallNode(
      std::unique_ptr<ExprNode> operand,
      const real_talk::lexer::TokenInfo &op_start_token,
      std::vector< std::unique_ptr<ExprNode> > args,
      const std::vector<real_talk::lexer::TokenInfo> &arg_separator_tokens,
      const real_talk::lexer::TokenInfo &op_end_token)
      : operand_(move(operand)),
        op_start_token_(op_start_token),
        args_(move(args)),
        arg_separator_tokens_(arg_separator_tokens),
        op_end_token_(op_end_token) {
    assert(operand_);
    assert((arg_separator_tokens_.empty() && args_.empty())
           || (arg_separator_tokens_.size() == args_.size() - 1));
  }

  virtual const real_talk::lexer::TokenInfo &GetStartToken() const override {
    return operand_->GetStartToken();
  }

  const std::vector< std::unique_ptr<ExprNode> > &GetArgs() const {
    return args_;
  }

  const std::unique_ptr<ExprNode> &GetOperand() const {
    return operand_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitCall(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const CallNode &call_node = static_cast<const CallNode&>(node);
    return op_start_token_ == call_node.op_start_token_
        && op_end_token_ == call_node.op_end_token_
        && arg_separator_tokens_.size() ==
        call_node.arg_separator_tokens_.size()
        && std::equal(
            arg_separator_tokens_.begin(),
            arg_separator_tokens_.end(),
            call_node.arg_separator_tokens_.begin())
        && *operand_ == *(call_node.operand_)
        && args_.size() == call_node.args_.size()
        && std::equal(
            boost::make_indirect_iterator(args_.begin()),
            boost::make_indirect_iterator(args_.end()),
            boost::make_indirect_iterator(call_node.args_.begin()));
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *operand_ << op_start_token_.GetValue();

    if (!args_.empty()) {
      auto last_arg_it = args_.cend() - 1;
      auto arg_separator_it = arg_separator_tokens_.cbegin();

      for (auto arg_it = args_.begin(); arg_it != last_arg_it; ++arg_it) {
        stream << **arg_it << arg_separator_it->GetValue() << ' ';
        ++arg_separator_it;
      }

      stream << **last_arg_it;
    }

    stream << op_end_token_.GetValue();
  }

  std::unique_ptr<ExprNode> operand_;
  real_talk::lexer::TokenInfo op_start_token_;
  std::vector< std::unique_ptr<ExprNode> > args_;
  std::vector<real_talk::lexer::TokenInfo> arg_separator_tokens_;
  real_talk::lexer::TokenInfo op_end_token_;
};
}
}
#endif
