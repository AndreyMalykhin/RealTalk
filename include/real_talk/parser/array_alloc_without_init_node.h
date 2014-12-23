
#ifndef _REAL_TALK_PARSER_ARRAY_ALLOC_WITHOUT_INIT_NODE_H_
#define _REAL_TALK_PARSER_ARRAY_ALLOC_WITHOUT_INIT_NODE_H_

#include <vector>
#include "real_talk/parser/array_alloc_node.h"
#include "real_talk/parser/base_array_alloc_node.h"

namespace real_talk {
namespace parser {

class ArrayAllocWithoutInitNode: public ArrayAllocNode {
 public:
  ArrayAllocWithoutInitNode(
      const real_talk::lexer::TokenInfo &op_token,
      std::unique_ptr<PrimitiveDataTypeNode> data_type,
      std::vector< std::unique_ptr<ArraySizeNode> > sizes)
      : array_alloc_node_(op_token, move(data_type), move(sizes)) {
  }

  const std::vector< std::unique_ptr<ArraySizeNode> > &GetSizes()
      const override {
    return array_alloc_node_.GetSizes();
  }

  const std::unique_ptr<PrimitiveDataTypeNode> &GetDataType() const override {
    return array_alloc_node_.GetDataType();
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitArrayAllocWithoutInit(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ArrayAllocWithoutInitNode &rhs =
        static_cast<const ArrayAllocWithoutInitNode&>(node);
    return array_alloc_node_ == rhs.array_alloc_node_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << array_alloc_node_;
  }

  BaseArrayAllocNode array_alloc_node_;
};
}
}
#endif
