
#ifndef _REAL_TALK_PARSER_ARRAY_ALLOC_WITHOUT_INIT_NODE_H_
#define _REAL_TALK_PARSER_ARRAY_ALLOC_WITHOUT_INIT_NODE_H_

#include <boost/iterator/indirect_iterator.hpp>
#include <vector>
#include <limits>
#include "real_talk/parser/array_alloc_node.h"
#include "real_talk/parser/base_array_alloc_node.h"
#include "real_talk/parser/bounded_array_size_node.h"

namespace real_talk {
namespace parser {

class ArrayAllocWithoutInitNode: public ArrayAllocNode {
 public:
  ArrayAllocWithoutInitNode(
      const real_talk::lexer::TokenInfo &op_token,
      std::unique_ptr<PrimitiveDataTypeNode> data_type,
      std::vector< std::unique_ptr<BoundedArraySizeNode> > sizes)
      : array_alloc_node_(op_token, move(data_type)),
        sizes_(move(sizes)) {
    assert(!sizes_.empty());
  }

  const std::vector< std::unique_ptr<BoundedArraySizeNode> > &GetSizes() const {
    return sizes_;
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
    return array_alloc_node_ == rhs.array_alloc_node_
        && sizes_.size() == rhs.sizes_.size()
        && std::equal(
            boost::make_indirect_iterator(sizes_.begin()),
            boost::make_indirect_iterator(sizes_.end()),
            boost::make_indirect_iterator(rhs.sizes_.begin()));
  }

  virtual void Print(std::ostream &stream) const override {
    stream << array_alloc_node_;

    for (const std::unique_ptr<BoundedArraySizeNode> &size: sizes_) {
      stream << *size;
    }
  }

  BaseArrayAllocNode array_alloc_node_;
  std::vector< std::unique_ptr<BoundedArraySizeNode> > sizes_;
};
}
}
#endif
