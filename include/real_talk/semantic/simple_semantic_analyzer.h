
#ifndef _REAL_TALK_SEMANTIC_SIMPLE_SEMANTIC_ANALYZER_H_
#define _REAL_TALK_SEMANTIC_SIMPLE_SEMANTIC_ANALYZER_H_

#include <memory>
#include <stdexcept>
#include "real_talk/semantic/semantic_analysis.h"

namespace real_talk {
namespace parser {

class ProgramNode;
class ParserFactory;
}

namespace semantic {

class SemanticErrorException: public std::runtime_error {
 public:
  explicit SemanticErrorException(std::unique_ptr<SemanticError> error);
  std::unique_ptr<SemanticError> GetError();
 private:
  std::unique_ptr<SemanticError> error_;
};

class SimpleSemanticAnalyzer {
 public:
  explicit SimpleSemanticAnalyzer(
      const real_talk::parser::ProgramNode &program,
      const real_talk::parser::ParserFactory &parser_factory);
  ~SimpleSemanticAnalyzer();
  SemanticAnalysis Analyze();

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
#endif
