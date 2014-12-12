
#ifndef _REAL_TALK_PARSER_EXPR_STMT_NODE_H_
#define _REAL_TALK_PARSER_EXPR_STMT_NODE_H_

#include <memory>
#include "real_talk/parser/stmt_node.h"
#include "real_talk/parser/expr_node.h"

namespace real_talk {
namespace parser {

class ExprStmtNode: public StmtNode {
 public:
  explicit ExprStmtNode(
      std::unique_ptr<ExprNode> expr,
      const real_talk::lexer::TokenInfo &end_token)
      : expr_(move(expr)),
        end_token_(end_token) {
    assert(expr_);
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitExprStmt(*this);
  }

  const std::unique_ptr<ExprNode> &GetExpr() const {
    return expr_;
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ExprStmtNode &stmt_node = static_cast<const ExprStmtNode&>(node);
    return end_token_ == stmt_node.end_token_ && *expr_ == *(stmt_node.expr_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << *expr_ << end_token_.GetValue();
  }

  std::unique_ptr<ExprNode> expr_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}

#endif
