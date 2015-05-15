
#ifndef _REAL_TALK_COMPILER_COMPILER_CONFIG_PARSER_H_
#define _REAL_TALK_COMPILER_COMPILER_CONFIG_PARSER_H_

namespace real_talk {
namespace compiler {

class CompilerConfig;

class CompilerConfigParser {
 public:
  virtual ~CompilerConfigParser() {}
  virtual void Parse(int argc, const char *argv[], CompilerConfig *config)
      const = 0;
};
}
}
#endif
