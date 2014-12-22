
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_ANALYSIS_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_ANALYSIS_H_

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "real_talk/semantic/semantic_problem.h"
#include "real_talk/semantic/def_analysis.h"
#include "real_talk/semantic/expr_analysis.h"
#include "real_talk/semantic/lit_analysis.h"
#include "real_talk/semantic/id_analysis.h"
#include "real_talk/semantic/import_analysis.h"

namespace real_talk {
namespace parser {

class ExprNode;
class DefNode;
class LitNode;
class IdNode;
class ImportNode;
}

namespace semantic {

class SemanticAnalysis {
 public:
  typedef std::unordered_map<const real_talk::parser::DefNode*,
                             std::unique_ptr<DefAnalysis> > DefAnalyzes;
  typedef std::unordered_map<const real_talk::parser::ExprNode*,
                             ExprAnalysis> ExprAnalyzes;
  typedef std::unordered_map<const real_talk::parser::LitNode*,
                             LitAnalysis> LitAnalyzes;
  typedef std::unordered_map<const real_talk::parser::ImportNode*,
                             ImportAnalysis> ImportAnalyzes;
  typedef std::unordered_map<const real_talk::parser::IdNode*,
                             IdAnalysis> IdAnalyzes;

  SemanticAnalysis(
      std::vector< std::unique_ptr<SemanticProblem> > problems,
      DefAnalyzes def_analyzes,
      ExprAnalyzes expr_analyzes,
      LitAnalyzes lit_analyzes,
      ImportAnalyzes import_analyzes,
      const IdAnalyzes &id_analyzes);
  friend bool operator==(const SemanticAnalysis &lhs,
                         const SemanticAnalysis &rhs);
  friend std::ostream &operator<<(std::ostream &stream,
                                  const SemanticAnalysis &result);

 private:
  std::vector< std::unique_ptr<SemanticProblem> > problems_;
  DefAnalyzes def_analyzes_;
  ExprAnalyzes expr_analyzes_;
  LitAnalyzes lit_analyzes_;
  ImportAnalyzes import_analyzes_;
  IdAnalyzes id_analyzes_;
};
}
}
#endif
