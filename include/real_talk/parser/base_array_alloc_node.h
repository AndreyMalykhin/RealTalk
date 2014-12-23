
#ifndef _REAL_TALK_PARSER_BASE_ARRAY_ALLOC_NODE_H_
#define _REAL_TALK_PARSER_BASE_ARRAY_ALLOC_NODE_H_

#include <boost/iterator/indirect_iterator.hpp>
#include <vector>
#include <memory>
#include "real_talk/parser/primitive_data_type_node.h"
#include "real_talk/parser/array_size_node.h"

namespace real_talk {
namespace parser {

class BaseArrayAllocNode final {
 public:
  BaseArrayAllocNode(
      const real_talk::lexer::TokenInfo &op_token,
      std::unique_ptr<PrimitiveDataTypeNode> data_type,
      std::vector< std::unique_ptr<ArraySizeNode> > sizes)
      : op_token_(op_token),
      data_type_(move(data_type)),
      sizes_(move(sizes)) {
    assert(data_type_);
    assert(!sizes_.empty());
  }

  const std::vector< std::unique_ptr<ArraySizeNode> > &GetSizes() const {
    return sizes_;
  }

  const std::unique_ptr<PrimitiveDataTypeNode> &GetDataType() const {
    return data_type_;
  }

  friend bool operator==(const BaseArrayAllocNode &lhs, const BaseArrayAllocNode &rhs) {
    return lhs.op_token_ == rhs.op_token_
        && *(lhs.data_type_) == *(rhs.data_type_)
        && lhs.sizes_.size() == rhs.sizes_.size()
        && std::equal(boost::make_indirect_iterator(lhs.sizes_.begin()),
                      boost::make_indirect_iterator(lhs.sizes_.end()),
                      boost::make_indirect_iterator(rhs.sizes_.begin()));
  }

  friend std::ostream &operator<<(std::ostream &stream,
                                  const BaseArrayAllocNode &node) {
    stream << node.op_token_.GetValue() << ' ' << *(node.data_type_);

    for (const std::unique_ptr<ArraySizeNode> &size: node.sizes_) {
      stream << *size;
    }

    return stream;
  }

 private:
  real_talk::lexer::TokenInfo op_token_;
  std::unique_ptr<PrimitiveDataTypeNode> data_type_;
  std::vector< std::unique_ptr<ArraySizeNode> > sizes_;
};
}
}
#endif
