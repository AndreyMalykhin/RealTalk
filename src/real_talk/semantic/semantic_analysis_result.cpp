
#include <boost/iterator/indirect_iterator.hpp>
#include <cassert>
#include <string>
#include <vector>
#include <utility>
#include "real_talk/parser/expr_node.h"
#include "real_talk/parser/def_node.h"
#include "real_talk/parser/id_node.h"
#include "real_talk/parser/lit_node.h"
#include "real_talk/semantic/semantic_analysis_result.h"
#include "real_talk/semantic/data_type.h"
#include "real_talk/semantic/lit.h"

using std::string;
using std::vector;
using std::unique_ptr;
using std::unordered_map;
using std::equal;
using std::pair;
using std::ostream;
using boost::make_indirect_iterator;
using real_talk::parser::ExprNode;
using real_talk::parser::DefNode;

namespace real_talk {
namespace semantic {

SemanticAnalysisResult::SemanticAnalysisResult(
    vector< unique_ptr<SemanticError> > errors,
    DefAnalyzes def_analyzes,
    ExprAnalyzes expr_analyzes,
    LitAnalyzes lit_analyzes,
    const IdAnalyzes &id_analyzes)
    : errors_(move(errors)),
      def_analyzes_(move(def_analyzes)),
      expr_analyzes_(move(expr_analyzes)),
      lit_analyzes_(move(lit_analyzes)),
      id_analyzes_(id_analyzes) {
}

IdAnalysis::IdAnalysis(const DefNode* def): def_(def) {
  assert(def_);
}

DefAnalysis::DefAnalysis(unique_ptr<DataType> data_type)
    : data_type_(move(data_type)) {
  assert(data_type_);
}

ExprAnalysis::ExprAnalysis(unique_ptr<DataType> data_type)
    : data_type_(move(data_type)) {
  assert(data_type_);
}

LitAnalysis::LitAnalysis(unique_ptr<Lit> lit): lit_(move(lit)) {
  assert(lit_);
}

const Lit &LitAnalysis::GetLit() const {
  return *lit_;
}

const DataType &ExprAnalysis::GetDataType() const {
  return *data_type_;
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

bool operator==(const DefAnalysis &lhs, const DefAnalysis &rhs) {
  return *(lhs.data_type_) == *(rhs.data_type_);
}

bool operator==(const ExprAnalysis &lhs, const ExprAnalysis &rhs) {
  return *(lhs.data_type_) == *(rhs.data_type_);
}

bool operator==(const LitAnalysis &lhs, const LitAnalysis &rhs) {
  return *(lhs.lit_) == *(rhs.lit_);
}

bool operator==(const IdAnalysis &lhs, const IdAnalysis &rhs) {
  return lhs.def_ == rhs.def_;
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
      && lhs.def_analyzes_.size() == rhs.def_analyzes_.size()
      && lhs.expr_analyzes_.size() == rhs.expr_analyzes_.size()
      && lhs.lit_analyzes_.size() == rhs.lit_analyzes_.size()
      && lhs.id_analyzes_.size() == rhs.id_analyzes_.size()
      && equal(make_indirect_iterator(lhs.errors_.begin()),
               make_indirect_iterator(lhs.errors_.end()),
               make_indirect_iterator(rhs.errors_.begin()))
      && equal(lhs.def_analyzes_.begin(),
               lhs.def_analyzes_.end(),
               rhs.def_analyzes_.begin())
      && equal(lhs.expr_analyzes_.begin(),
               lhs.expr_analyzes_.end(),
               rhs.expr_analyzes_.begin())
      && equal(lhs.lit_analyzes_.begin(),
               lhs.lit_analyzes_.end(),
               rhs.lit_analyzes_.begin())
      && equal(lhs.id_analyzes_.begin(),
               lhs.id_analyzes_.end(),
               rhs.id_analyzes_.begin());
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

ostream &operator<<(ostream &stream, const DefAnalysis &def_analysis) {
  return stream << "data_type=" << *(def_analysis.data_type_);
}

ostream &operator<<(ostream &stream, const ExprAnalysis &expr_analysis) {
  return stream << "data_type=" << *(expr_analysis.data_type_);
}

ostream &operator<<(ostream &stream, const LitAnalysis &lit_analysis) {
  return stream << "value=" << *(lit_analysis.lit_);
}

ostream &operator<<(ostream &stream, const IdAnalysis &id_analysis) {
  return stream << "def=" << *(id_analysis.def_);
}

ostream &operator<<(ostream &stream, const SemanticAnalysisResult &result) {
  for (const unique_ptr<SemanticError> &error: result.errors_) {
    stream << "error=" << *error << "\n";
  }

  for (const auto &pair: result.def_analyzes_) {
    stream << "def=" << *(pair.first) << "; analysis=" << pair.second << "\n";
  }

  for (const auto &pair: result.expr_analyzes_) {
    stream << "expr=" << *(pair.first) << "; analysis=" << pair.second << "\n";
  }

  for (const auto &pair: result.lit_analyzes_) {
    stream << "lit=" << *(pair.first) << "; analysis=" << pair.second << "\n";
  }

  for (const auto &pair: result.id_analyzes_) {
    stream << "id=" << *(pair.first) << "; analysis=" << pair.second << "\n";
  }

  return stream;
}
}
}
