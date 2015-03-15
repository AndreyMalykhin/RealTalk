
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_ANALYSIS_H_

#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>
#include "real_talk/semantic/semantic_problem.h"

namespace real_talk {
namespace parser {

class Node;
}

namespace semantic {

class NodeSemanticAnalysis;

class SemanticAnalysis {
 public:
  typedef std::unordered_map <const real_talk::parser::Node*,
                              std::unique_ptr<NodeSemanticAnalysis> > NodeAnalyzes;
  typedef std::vector< std::unique_ptr<SemanticProblem> > Problems;

  SemanticAnalysis(
      std::vector< std::unique_ptr<SemanticProblem> > problems,
      NodeAnalyzes node_analyzes);
  const NodeAnalyzes &GetNodeAnalyzes() const;
  const Problems &GetProblems() const;
  friend bool operator==(const SemanticAnalysis &lhs,
                         const SemanticAnalysis &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const SemanticAnalysis &analysis);

 private:
  Problems problems_;
  NodeAnalyzes node_analyzes_;
};
}
}
#endif
