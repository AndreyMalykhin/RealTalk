
#include <boost/iterator/indirect_iterator.hpp>
#include <vector>
#include <utility>
#include "real_talk/parser/def_node.h"
#include "real_talk/parser/expr_node.h"
#include "real_talk/parser/lit_node.h"
#include "real_talk/parser/id_node.h"
#include "real_talk/parser/import_node.h"
#include "real_talk/semantic/semantic_analysis.h"

using std::vector;
using std::unique_ptr;
using std::equal;
using std::pair;
using std::ostream;
using boost::make_indirect_iterator;

namespace real_talk {
namespace semantic {

SemanticAnalysis::SemanticAnalysis(
    vector< unique_ptr<SemanticProblem> > problems,
    DefAnalyzes def_analyzes,
    ExprAnalyzes expr_analyzes,
    LitAnalyzes lit_analyzes,
    ImportAnalyzes import_analyzes,
    const IdAnalyzes &id_analyzes)
    : problems_(move(problems)),
      def_analyzes_(move(def_analyzes)),
      expr_analyzes_(move(expr_analyzes)),
      lit_analyzes_(move(lit_analyzes)),
      import_analyzes_(move(import_analyzes)),
      id_analyzes_(id_analyzes) {
}

bool operator==(const SemanticAnalysis &lhs, const SemanticAnalysis &rhs) {
  static auto def_analysis_comparator = [](
      const SemanticAnalysis::DefAnalyzes::value_type &lhs_pair,
      const SemanticAnalysis::DefAnalyzes::value_type &rhs_pair) {
    return lhs_pair.first == rhs_pair.first
    && *(lhs_pair.second) == *(rhs_pair.second);
  };

  return lhs.problems_.size() == rhs.problems_.size()
      && lhs.def_analyzes_.size() == rhs.def_analyzes_.size()
      && lhs.expr_analyzes_.size() == rhs.expr_analyzes_.size()
      && lhs.lit_analyzes_.size() == rhs.lit_analyzes_.size()
      && lhs.id_analyzes_.size() == rhs.id_analyzes_.size()
      && equal(make_indirect_iterator(lhs.problems_.begin()),
               make_indirect_iterator(lhs.problems_.end()),
               make_indirect_iterator(rhs.problems_.begin()))
      && equal(lhs.def_analyzes_.begin(),
               lhs.def_analyzes_.end(),
               rhs.def_analyzes_.begin(),
               def_analysis_comparator)
      && equal(lhs.expr_analyzes_.begin(),
               lhs.expr_analyzes_.end(),
               rhs.expr_analyzes_.begin())
      && equal(lhs.lit_analyzes_.begin(),
               lhs.lit_analyzes_.end(),
               rhs.lit_analyzes_.begin())
      && equal(lhs.import_analyzes_.begin(),
               lhs.import_analyzes_.end(),
               rhs.import_analyzes_.begin())
      && equal(lhs.id_analyzes_.begin(),
               lhs.id_analyzes_.end(),
               rhs.id_analyzes_.begin());
}

ostream &operator<<(ostream &stream, const SemanticAnalysis &result) {
  for (const unique_ptr<SemanticProblem> &problem: result.problems_) {
    stream << "problem=" << *problem << "\n";
  }

  for (const auto &pair: result.def_analyzes_) {
    stream << "def=" << *(pair.first) << "; analysis=" << *(pair.second)
           << "\n";
  }

  for (const auto &pair: result.expr_analyzes_) {
    stream << "expr=" << *(pair.first) << "; analysis=" << pair.second << "\n";
  }

  for (const auto &pair: result.lit_analyzes_) {
    stream << "lit=" << *(pair.first) << "; analysis=" << pair.second << "\n";
  }

  for (const auto &pair: result.import_analyzes_) {
    stream << "import=" << *(pair.first) << "; analysis=" << pair.second
           << "\n";
  }

  for (const auto &pair: result.id_analyzes_) {
    stream << "id=" << *(pair.first) << "; analysis=" << pair.second << "\n";
  }

  return stream;
}
}
}
