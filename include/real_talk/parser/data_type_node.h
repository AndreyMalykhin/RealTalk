
#ifndef _REAL_TALK_PARSER_DATA_TYPE_NODE_H_
#define _REAL_TALK_PARSER_DATA_TYPE_NODE_H_

#include "real_talk/parser/node.h"

namespace real_talk {
namespace lexer {

class TokenInfo;
}

namespace parser {

class DataTypeNode: public Node {
 public:
  virtual const real_talk::lexer::TokenInfo &GetNameToken() const = 0;
};
}
}
#endif
