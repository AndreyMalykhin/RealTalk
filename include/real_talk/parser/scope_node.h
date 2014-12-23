
#ifndef _REAL_TALK_PARSER_SCOPE_NODE_H_
#define _REAL_TALK_PARSER_SCOPE_NODE_H_

#include <boost/iterator/indirect_iterator.hpp>
#include <vector>
#include <memory>
#include "real_talk/parser/node.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class ScopeNode final {
 public:
  ScopeNode(
      const real_talk::lexer::TokenInfo &start_token,
      std::vector< std::unique_ptr<StmtNode> > stmts,
      const real_talk::lexer::TokenInfo &end_token)
      : start_token_(start_token),
        stmts_(move(stmts)),
        end_token_(end_token) {
  }

  const std::vector< std::unique_ptr<StmtNode> > &GetStmts() const {
    return stmts_;
  }

  friend bool operator==(const ScopeNode &lhs, const ScopeNode &rhs) {
    return lhs.start_token_ == rhs.start_token_
        && lhs.end_token_ == rhs.end_token_
        && lhs.stmts_.size() == rhs.stmts_.size()
        && std::equal(boost::make_indirect_iterator(lhs.stmts_.begin()),
                      boost::make_indirect_iterator(lhs.stmts_.end()),
                      boost::make_indirect_iterator(rhs.stmts_.begin()));
  }

  friend std::ostream &operator<<(std::ostream &stream, const ScopeNode &node) {
    stream << node.start_token_.GetValue() << '\n';

    for (const std::unique_ptr<StmtNode> &stmt: node.stmts_) {
      stream << *stmt << '\n';
    }

    return stream << node.end_token_.GetValue();
  }

 private:
  real_talk::lexer::TokenInfo start_token_;
  std::vector< std::unique_ptr<StmtNode> > stmts_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}

#endif
