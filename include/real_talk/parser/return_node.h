
#ifndef _REAL_TALK_PARSER_RETURN_NODE_H_
#define _REAL_TALK_PARSER_RETURN_NODE_H_

#include "real_talk/parser/stmt_node.h"

namespace real_talk {
namespace parser {

class ReturnNode: public StmtNode {
 public:
  virtual const real_talk::lexer::TokenInfo &GetStartToken() const = 0;
  virtual const real_talk::lexer::TokenInfo &GetEndToken() const = 0;
};
}
}
#endif
