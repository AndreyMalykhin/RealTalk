
#include <boost/iterator/indirect_iterator.hpp>
#include <vector>
#include <utility>
#include "real_talk/parser/node.h"
#include "real_talk/semantic/node_semantic_analysis.h"
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
    Problems problems, NodeAnalyzes node_analyzes)
    : problems_(move(problems)), node_analyzes_(move(node_analyzes)) {
}

const SemanticAnalysis::NodeAnalyzes &SemanticAnalysis::GetNodeAnalyzes()
    const {
  return node_analyzes_;
}

const SemanticAnalysis::Problems &SemanticAnalysis::GetProblems() const {
  return problems_;
}

bool operator==(const SemanticAnalysis &lhs, const SemanticAnalysis &rhs) {
  const auto node_analyzes_comparator = [&rhs](
      const SemanticAnalysis::NodeAnalyzes::value_type &lhs_pair) {
    SemanticAnalysis::NodeAnalyzes::const_iterator rhs_pair_it =
    rhs.node_analyzes_.find(lhs_pair.first);
    return rhs_pair_it != rhs.node_analyzes_.end()
    && *(rhs_pair_it->second) == *(lhs_pair.second);
  };

  return lhs.problems_.size() == rhs.problems_.size()
      && lhs.node_analyzes_.size() == rhs.node_analyzes_.size()
      && equal(make_indirect_iterator(lhs.problems_.begin()),
               make_indirect_iterator(lhs.problems_.end()),
               make_indirect_iterator(rhs.problems_.begin()))
      && all_of(lhs.node_analyzes_.begin(),
                lhs.node_analyzes_.end(),
                node_analyzes_comparator);
}

ostream &operator<<(ostream &stream, const SemanticAnalysis &result) {
  for (const unique_ptr<SemanticProblem> &problem: result.problems_) {
    stream << "problem=" << *problem << "\n";
  }

  for (const SemanticAnalysis::NodeAnalyzes::value_type &node_analysis
           : result.node_analyzes_) {
    stream << "node=" << *(node_analysis.first) << "; analysis="
           << *(node_analysis.second) << "\n";
  }

  return stream;
}
}
}
