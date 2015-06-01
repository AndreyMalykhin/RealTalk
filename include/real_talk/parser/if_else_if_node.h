
#ifndef _REAL_TALK_PARSER_IF_ELSE_IF_NODE_H_
#define _REAL_TALK_PARSER_IF_ELSE_IF_NODE_H_

#include <boost/iterator/indirect_iterator.hpp>
#include <memory>
#include <vector>
#include "real_talk/parser/branch_node.h"
#include "real_talk/parser/if_node.h"
#include "real_talk/parser/else_if_node.h"
#include "real_talk/parser/node_visitor.h"
#include "real_talk/lexer/token_info.h"

namespace real_talk {
namespace parser {

class IfElseIfNode: public BranchNode {
 public:
  IfElseIfNode(
      std::unique_ptr<IfNode> if_node,
      std::vector< std::unique_ptr<ElseIfNode> > else_if_nodes)
      : if_(move(if_node)),
        else_ifs_(move(else_if_nodes)) {
    assert(if_);
  }

  virtual const real_talk::lexer::TokenInfo &GetStartToken() const override {
    return if_->GetStartToken();
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitIfElseIf(*this);
  }

  const std::unique_ptr<IfNode> &GetIf() const override {
    return if_;
  }

  const std::vector< std::unique_ptr<ElseIfNode> > &GetElseIfs()
      const override {
    return else_ifs_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const IfElseIfNode &rhs = static_cast<const IfElseIfNode&>(node);
    return *if_ == *(rhs.if_)
        && else_ifs_.size() == rhs.else_ifs_.size()
        && std::equal(boost::make_indirect_iterator(else_ifs_.begin()),
                      boost::make_indirect_iterator(else_ifs_.end()),
                      boost::make_indirect_iterator(rhs.else_ifs_.begin()));
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *if_;

    for (const std::unique_ptr<ElseIfNode> &else_if: else_ifs_) {
      stream << ' ' << *else_if;
    }
  }

  std::unique_ptr<IfNode> if_;
  std::vector< std::unique_ptr<ElseIfNode> > else_ifs_;
};
}
}

#endif
