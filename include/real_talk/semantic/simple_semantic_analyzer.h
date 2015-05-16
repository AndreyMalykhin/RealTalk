
#ifndef _REAL_TALK_SEMANTIC_SIMPLE_SEMANTIC_ANALYZER_H_
#define _REAL_TALK_SEMANTIC_SIMPLE_SEMANTIC_ANALYZER_H_

#include <vector>
#include "real_talk/semantic/semantic_analyzer.h"

namespace real_talk {
namespace parser {

class ProgramNode;
}

namespace semantic {

class LitParser;

class SimpleSemanticAnalyzer: public SemanticAnalyzer {
 public:
  explicit SimpleSemanticAnalyzer(const LitParser &lit_parser);
  virtual ~SimpleSemanticAnalyzer() override;
  virtual std::unique_ptr<SemanticAnalysis> Analyze(
      const real_talk::parser::ProgramNode &main_program,
      const ImportPrograms &import_programs) override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
#endif
