
#ifndef _REAL_TALK_PARSER_FUNC_DEF_NODE_H_
#define _REAL_TALK_PARSER_FUNC_DEF_NODE_H_

#include <memory>
#include <vector>
#include "real_talk/parser/stmt_node.h"
#include "real_talk/parser/def_node.h"

namespace real_talk {
namespace lexer {

class TokenInfo;
}

namespace parser {

class DataTypeNode;
class ArgDefNode;

class FuncDefNode: public StmtNode, public DefNode {
 public:
  virtual const std::vector<real_talk::lexer::TokenInfo>
  &GetModifierTokens() const = 0;

  virtual const real_talk::lexer::TokenInfo &GetArgsStartToken() const = 0;

  virtual const std::vector< std::unique_ptr<ArgDefNode> >
  &GetArgs() const = 0;

  virtual const std::vector<real_talk::lexer::TokenInfo>
  &GetArgSeparatorTokens() const  = 0;

  virtual const real_talk::lexer::TokenInfo &GetArgsEndToken() const = 0;
};
}
}
#endif
