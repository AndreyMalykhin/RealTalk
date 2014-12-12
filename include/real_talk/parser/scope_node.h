
#ifndef _REAL_TALK_PARSER_SCOPE_NODE_H_
#define _REAL_TALK_PARSER_SCOPE_NODE_H_

#include <boost/iterator/indirect_iterator.hpp>
#include <vector>
#include <memory>
#include "real_talk/parser/node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class ScopeNode: public Node {
 public:
  ScopeNode(
      const real_talk::lexer::TokenInfo &start_token,
      std::vector< std::unique_ptr<StmtNode> > stmts,
      const real_talk::lexer::TokenInfo &end_token)
      : start_token_(start_token),
        stmts_(move(stmts)),
        end_token_(end_token) {
  }

  virtual void Accept(NodeVisitor&) const override {
    assert(false);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ScopeNode &rhs = static_cast<const ScopeNode&>(node);
    return start_token_ == rhs.start_token_
        && end_token_ == rhs.end_token_
        && stmts_.size() == rhs.stmts_.size()
        && std::equal(boost::make_indirect_iterator(stmts_.begin()),
                      boost::make_indirect_iterator(stmts_.end()),
                      boost::make_indirect_iterator(rhs.stmts_.begin()));
  }

  virtual void Print(std::ostream &stream) const override {
    stream << start_token_.GetValue() << '\n';

    for (const std::unique_ptr<StmtNode> &stmt: stmts_) {
      stream << *stmt << '\n';
    }

    stream << end_token_.GetValue();
  }

  real_talk::lexer::TokenInfo start_token_;
  std::vector< std::unique_ptr<StmtNode> > stmts_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}

#endif
