
#ifndef _REAL_TALK_CODE_CODE_GENERATOR_H_
#define _REAL_TALK_CODE_CODE_GENERATOR_H_

#include <cstdint>

namespace real_talk {
namespace parser {

class ProgramNode;
}

namespace semantic {

class SemanticAnalysis;
}

namespace code {

class Code;

class CodeGenerator {
 public:
  virtual ~CodeGenerator() {}

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  virtual void Generate(
      const real_talk::parser::ProgramNode &program,
      const real_talk::semantic::SemanticAnalysis &semantic_analysis,
      uint32_t version,
      Code *output) = 0;
};
}
}
#endif
