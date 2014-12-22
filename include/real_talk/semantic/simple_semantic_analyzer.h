
#ifndef _REAL_TALK_SEMANTIC_SIMPLE_SEMANTIC_ANALYZER_H_
#define _REAL_TALK_SEMANTIC_SIMPLE_SEMANTIC_ANALYZER_H_

#include <memory>
#include "real_talk/semantic/semantic_analysis.h"

namespace real_talk {
namespace parser {

class ProgramNode;
class FileParser;
}

namespace semantic {

class ImportFileSearcher;
class LitParser;

class SimpleSemanticAnalyzer {
 public:
  explicit SimpleSemanticAnalyzer(
      const real_talk::parser::ProgramNode &program,
      const real_talk::parser::FileParser &file_parser,
      const ImportFileSearcher &import_file_searcher,
      const LitParser &lit_parser);
  ~SimpleSemanticAnalyzer();
  SemanticAnalysis Analyze();

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
#endif
