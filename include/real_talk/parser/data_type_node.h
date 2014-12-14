
#ifndef _REAL_TALK_PARSER_DATA_TYPE_NODE_H_
#define _REAL_TALK_PARSER_DATA_TYPE_NODE_H_

#include <string>
#include <memory>
#include "real_talk/parser/node.h"

namespace real_talk {
namespace parser {

class DataTypeNode: public Node {
 public:
  virtual std::unique_ptr<DataTypeNode> Clone() const = 0;
};
}
}
#endif
