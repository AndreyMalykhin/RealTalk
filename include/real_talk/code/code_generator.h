
#ifndef _REAL_TALK_CODE_CODE_GENERATOR_H_
#define _REAL_TALK_CODE_CODE_GENERATOR_H_

#include <cstdint>
#include <iostream>
#include <memory>

namespace real_talk {
namespace parser {

class ProgramNode;
}

namespace semantic {

class SemanticAnalysis;
}

namespace code {

class CastCmdGenerator;

class CodeGenerator {
 public:
  explicit CodeGenerator(const CastCmdGenerator &cast_cmd_generator);
  ~CodeGenerator();
  void Generate(const real_talk::parser::ProgramNode &program,
                const real_talk::semantic::SemanticAnalysis &semantic_analysis,
                uint32_t version,
                std::ostream &stream);

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
#endif
