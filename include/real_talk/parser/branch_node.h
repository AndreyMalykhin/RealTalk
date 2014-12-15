
#ifndef _REAL_TALK_PARSER_BRANCH_NODE_H_
#define _REAL_TALK_PARSER_BRANCH_NODE_H_

#include <memory>
#include <vector>
#include "real_talk/parser/stmt_node.h"

namespace real_talk {
namespace parser {

class IfNode;
class ElseIfNode;

class BranchNode: public StmtNode {
 public:
  virtual const std::unique_ptr<IfNode> &GetIf() const = 0;
  virtual const std::vector< std::unique_ptr<ElseIfNode> > &GetElseIfs()
      const = 0;
};
}
}

#endif
