
#include <boost/iterator/indirect_iterator.hpp>
#include <string>
#include <vector>
#include <utility>
#include "real_talk/parser/expr_node.h"
#include "real_talk/semantic/semantic_analysis_result.h"
#include "real_talk/semantic/data_type.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::unordered_map;
using std::equal;
using std::pair;
using std::ostream;
using boost::make_indirect_iterator;
using real_talk::parser::ExprNode;

namespace real_talk {
namespace semantic {

SemanticAnalysisResult::SemanticAnalysisResult(
    vector< unique_ptr<SemanticError> > errors)
    : errors_(move(errors)) {
}

SemanticHint::SemanticHint(
    const string &message,
    uint32_t line_number,
    uint32_t column_number,
    const string &file_path)
    : message_(message),
      line_number_(line_number),
      column_number_(column_number),
      file_path_(file_path) {
}

SemanticError::SemanticError(
    const string &message,
    uint32_t line_number,
    uint32_t column_number,
    vector< unique_ptr<SemanticHint> > hints)
    : message_(message),
      line_number_(line_number),
      column_number_(column_number),
      hints_(move(hints)) {
}

bool operator==(const SemanticHint &lhs, const SemanticHint &rhs) {
  return lhs.message_ == rhs.message_
      && lhs.line_number_ == rhs.line_number_
      && lhs.column_number_ == rhs.column_number_
      && lhs.file_path_ == rhs.file_path_;
}

bool operator==(const SemanticError &lhs, const SemanticError &rhs) {
  return lhs.message_ == rhs.message_
      && lhs.line_number_ == rhs.line_number_
      && lhs.column_number_ == rhs.column_number_
      && lhs.hints_.size() == rhs.hints_.size()
      && equal(make_indirect_iterator(lhs.hints_.begin()),
               make_indirect_iterator(lhs.hints_.end()),
               make_indirect_iterator(rhs.hints_.begin()));
}

bool operator==(const SemanticAnalysisResult &lhs,
                const SemanticAnalysisResult &rhs) {
  return lhs.errors_.size() == rhs.errors_.size()
      && equal(make_indirect_iterator(lhs.errors_.begin()),
               make_indirect_iterator(lhs.errors_.end()),
               make_indirect_iterator(rhs.errors_.begin()));
}

ostream &operator<<(ostream &stream, const SemanticHint &hint) {
  return stream << "message=" << hint.message_ << "; line=" << hint.line_number_
                << "; column=" << hint.column_number_ << "; file_path="
                << hint.file_path_;
}

ostream &operator<<(ostream &stream, const SemanticError &error) {
  stream << "message=" << error.message_ << "; line="
         << error.line_number_ << "; column=" << error.column_number_
         << "; hints=\n";

  for (const unique_ptr<SemanticHint> &hint: error.hints_) {
    stream << *hint << "\n";
  }

  return stream;
}

ostream &operator<<(ostream &stream, const SemanticAnalysisResult &result) {
  for (const unique_ptr<SemanticError> &error: result.errors_) {
    stream << "error=" << *error << "\n";
  }

  return stream;
}
}
}
