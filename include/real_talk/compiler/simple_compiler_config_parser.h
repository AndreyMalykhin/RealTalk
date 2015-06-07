
#ifndef _REAL_TALK_COMPILER_SIMPLE_COMPILER_CONFIG_PARSER_H_
#define _REAL_TALK_COMPILER_SIMPLE_COMPILER_CONFIG_PARSER_H_

#include <string>
#include "real_talk/compiler/compiler_config_parser.h"

namespace real_talk {
namespace compiler {

class SimpleCompilerConfigParser: public CompilerConfigParser {
 public:
  virtual void Parse(int argc,
                     const char *argv[],
                     CompilerConfig *config,
                     CompilerCmd *cmd) const override;
  virtual std::string GetHelp() const override;
};
}
}
#endif
