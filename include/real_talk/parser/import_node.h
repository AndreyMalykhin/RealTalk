
#ifndef _REAL_TALK_PARSER_IMPORT_NODE_H_
#define _REAL_TALK_PARSER_IMPORT_NODE_H_

#include <cassert>
#include <string>
#include <memory>
#include "real_talk/parser/stmt_node.h"
#include "real_talk/parser/string_node.h"

namespace real_talk {
namespace parser {

class ImportNode: public StmtNode {
 public:
  ImportNode(
      const real_talk::lexer::TokenInfo &start_token,
      std::unique_ptr<StringNode> file_path,
      const real_talk::lexer::TokenInfo &end_token)
      : start_token_(start_token),
        file_path_(move(file_path)),
        end_token_(end_token) {
    assert(file_path_);
  }

  virtual const real_talk::lexer::TokenInfo &GetStartToken() const override {
    return start_token_;
  }

  const std::unique_ptr<StringNode> &GetFilePath() const {
    return file_path_;
  }

  virtual void Accept(NodeVisitor &visitor) const override {
    visitor.VisitImport(*this);
  }

 private:
  virtual bool IsEqual(const Node &node) const override {
    const ImportNode &rhs = static_cast<const ImportNode&>(node);
    return start_token_ == rhs.start_token_
        && end_token_ == rhs.end_token_
        && *file_path_ == *(rhs.file_path_);
  }

  virtual void Print(std::ostream &stream) const override {
    stream << start_token_.GetValue() << ' ' << *file_path_
           << end_token_.GetValue();
  }

  real_talk::lexer::TokenInfo start_token_;
  std::unique_ptr<StringNode> file_path_;
  real_talk::lexer::TokenInfo end_token_;
};
}
}
#endif
