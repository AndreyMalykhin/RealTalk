
#ifndef _REAL_TALK_PARSER_ARRAY_ALLOC_NODE_H_
#define _REAL_TALK_PARSER_ARRAY_ALLOC_NODE_H_

#include <memory>

namespace real_talk {
namespace parser {

class PrimitiveDataTypeNode;

class ArrayAllocNode: public ExprNode {
 public:
  virtual const std::unique_ptr<PrimitiveDataTypeNode> &GetDataType() const = 0;
};
}
}
#endif
