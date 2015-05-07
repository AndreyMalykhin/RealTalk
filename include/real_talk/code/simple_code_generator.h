
#ifndef _REAL_TALK_CODE_SIMPLE_CODE_GENERATOR_H_
#define _REAL_TALK_CODE_SIMPLE_CODE_GENERATOR_H_

#include <memory>
#include "real_talk/code/code_generator.h"

namespace real_talk {
namespace code {

class CastCmdGenerator;

class SimpleCodeGenerator: public CodeGenerator {
 public:
  explicit SimpleCodeGenerator(const CastCmdGenerator &cast_cmd_generator);
  virtual ~SimpleCodeGenerator() override;
  virtual void Generate(
      const real_talk::parser::ProgramNode &program,
      const real_talk::semantic::SemanticAnalysis &semantic_analysis,
      uint32_t version,
      Code *output) override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
#endif
