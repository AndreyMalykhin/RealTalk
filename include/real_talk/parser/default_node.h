
#ifndef _REAL_TALK_PARSER_DEFAULT_NODE_H_
#define _REAL_TALK_PARSER_DEFAULT_NODE_H_

#include "real_talk/parser/expr_node.h"

namespace real_talk {
namespace parser {

class DefaultNode: public ExprNode {
 private:
  virtual bool IsEqual(const Node&) const override {
    return true;
  }

  virtual void Print(std::ostream&) const override {
  }
};
}
}

#endif
