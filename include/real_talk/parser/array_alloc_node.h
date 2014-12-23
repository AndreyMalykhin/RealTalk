
#ifndef _REAL_TALK_PARSER_ARRAY_ALLOC_NODE_H_
#define _REAL_TALK_PARSER_ARRAY_ALLOC_NODE_H_

#include <vector>
#include <memory>
#include "real_talk/parser/expr_node.h"

namespace real_talk {
namespace parser {

class ArraySizeNode;
class PrimitiveDataTypeNode;

class ArrayAllocNode: public ExprNode {
 public:
  virtual const std::vector< std::unique_ptr<ArraySizeNode> > &GetSizes()
      const = 0;
  virtual const std::unique_ptr<PrimitiveDataTypeNode> &GetDataType() const = 0;
};
}
}
#endif
