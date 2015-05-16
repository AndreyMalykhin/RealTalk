
#ifndef _REAL_TALK_SEMANTIC_SEMANTIC_ANALYZER_H_
#define _REAL_TALK_SEMANTIC_SEMANTIC_ANALYZER_H_

#include <memory>
#include <vector>
#include "real_talk/semantic/semantic_analysis.h"

namespace real_talk {
namespace parser {

class ProgramNode;
}

namespace semantic {

class SemanticAnalyzer {
 public:
  typedef
  std::vector< std::unique_ptr<real_talk::parser::ProgramNode> > ImportPrograms;

  virtual ~SemanticAnalyzer() {}
  virtual std::unique_ptr<SemanticAnalysis> Analyze(
      const real_talk::parser::ProgramNode &main_program,
      const ImportPrograms &import_programs) = 0;
};
}
}
#endif
