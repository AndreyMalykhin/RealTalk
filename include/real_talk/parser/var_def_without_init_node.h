
#ifndef _REAL_TALK_PARSER_VAR_DEF_WITHOUT_INIT_NODE_H_
#define _REAL_TALK_PARSER_VAR_DEF_WITHOUT_INIT_NODE_H_

#include <string>
#include <memory>
#include "real_talk/parser/stmt_node.h"
#include "real_talk/parser/def_node.h"
#include "real_talk/parser/data_type_node.h"

namespace real_talk {
namespace parser {

class VarDefWithoutInitNode: public StmtNode, public DefNode {
 public:
  VarDefWithoutInitNode(
      std::unique_ptr<DataTypeNode> data_type,
      const real_talk::lexer::TokenInfo &name_token,
      const real_talk::lexer::TokenInfo &end_token)
      : data_type_(move(data_type)),
        name_token_(name_token),
        end_token_(end_token) {
    assert(data_type_);
  }

  const real_talk::lexer::TokenInfo &GetNameToken() const override {
    return name_token_;
  }

  const std::unique_ptr<DataTypeNode> &GetDataType() const override {
    return data_type_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitVarDefWithoutInit(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    return DoIsEqual(static_cast<const VarDefWithoutInitNode&>(node));
  }

  virtual bool IsEqual(const DefNode &node) const override {
    return DoIsEqual(static_cast<const VarDefWithoutInitNode&>(node));
  }

  bool DoIsEqual(const VarDefWithoutInitNode &rhs) const {
    return *data_type_ == *(rhs.data_type_)
        && name_token_ == rhs.name_token_
        && end_token_ == rhs.end_token_;
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *data_type_ << ' ' << name_token_.GetValue()
           << end_token_.GetValue();
  }

  std::unique_ptr<DataTypeNode> data_type_;
  real_talk::lexer::TokenInfo name_token_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}
#endif
