
#ifndef _REAL_TALK_COMPILER_COMPILER_CONFIG_PARSER_H_
#define _REAL_TALK_COMPILER_COMPILER_CONFIG_PARSER_H_

#include <cstdint>
#include <stdexcept>
#include <string>

namespace real_talk {
namespace compiler {

class CompilerConfig;
enum class CompilerCmd: uint8_t;

class CompilerConfigParser {
 public:
  class BadArgsError: public std::runtime_error {
   public:
    explicit BadArgsError(const std::string &msg): std::runtime_error(msg) {}
  };

  virtual ~CompilerConfigParser() {}

  /**
   * @throws real_talk::compiler::CompilerConfigParser::BadArgsError
   */
  virtual void Parse(int argc,
                     const char *argv[],
                     CompilerConfig *config,
                     CompilerCmd *cmd) const = 0;
  virtual std::string GetHelp() const = 0;
};
}
}
#endif
