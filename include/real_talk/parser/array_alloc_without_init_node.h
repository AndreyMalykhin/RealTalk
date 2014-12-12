
#ifndef _REAL_TALK_PARSER_ARRAY_ALLOC_WITHOUT_INIT_NODE_H_
#define _REAL_TALK_PARSER_ARRAY_ALLOC_WITHOUT_INIT_NODE_H_

#include "real_talk/parser/array_alloc_node.h"

namespace real_talk {
namespace parser {

class ArrayAllocWithoutInitNode: public ExprNode {
 public:
  ArrayAllocWithoutInitNode(
      const real_talk::lexer::TokenInfo &op_token,
      std::unique_ptr<SimpleDataTypeNode> data_type,
      const real_talk::lexer::TokenInfo &subscript_start_token,
      std::unique_ptr<ExprNode> size,
      const real_talk::lexer::TokenInfo &subscript_end_token)
      : array_alloc_node(op_token,
                         move(data_type),
                         subscript_start_token,
                         move(size),
                         subscript_end_token) {
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitArrayAllocWithoutInit(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ArrayAllocWithoutInitNode &rhs =
        static_cast<const ArrayAllocWithoutInitNode&>(node);
    return array_alloc_node.IsEqual(rhs.array_alloc_node);
  }

  virtual void Print(std::ostream &stream) const override {
    array_alloc_node.Print(stream);
  }

  ArrayAllocNode array_alloc_node;
};
}
}
#endif
