
#ifndef _REAL_TALK_PARSER_DEF_NODE_H_
#define _REAL_TALK_PARSER_DEF_NODE_H_

#include "real_talk/parser/stmt_node.h"

namespace real_talk {
namespace lexer {

class TokenInfo;
}

namespace parser {

class DataTypeNode;

class DefNode: public StmtNode {
 public:
  virtual const std::unique_ptr<DataTypeNode> &GetDataType() const = 0;
  virtual const real_talk::lexer::TokenInfo &GetNameToken() const = 0;
};
}
}
#endif
