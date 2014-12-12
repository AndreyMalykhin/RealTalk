
#ifndef _REAL_TALK_PARSER_PROGRAM_NODE_H_
#define _REAL_TALK_PARSER_PROGRAM_NODE_H_

#include <boost/iterator/indirect_iterator.hpp>
#include <vector>
#include <memory>
#include "real_talk/parser/node.h"
#include "real_talk/parser/stmt_node.h"
#include "real_talk/parser/node_visitor.h"

namespace real_talk {
namespace parser {

class ProgramNode: public Node {
 public:
  explicit ProgramNode(std::vector< std::unique_ptr<StmtNode> > stmts)
      : stmts_(move(stmts)) {
  }

  const std::vector< std::unique_ptr<StmtNode> > &GetStmts() const {
    return stmts_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitProgram(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ProgramNode &program_node = static_cast<const ProgramNode&>(node);
    return stmts_.size() == program_node.stmts_.size()
        && std::equal(
            boost::make_indirect_iterator(stmts_.begin()),
            boost::make_indirect_iterator(stmts_.end()),
            boost::make_indirect_iterator(program_node.stmts_.begin()));
  }

  virtual void Print(std::ostream &stream) const override {
    for (const std::unique_ptr<StmtNode> &stmt: stmts_) {
      stream << *stmt << '\n';
    }
  }

  std::vector< std::unique_ptr<StmtNode> > stmts_;
};
}
}
#endif
