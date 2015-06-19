
#ifndef _REAL_TALK_LINKER_LINKER_CONFIG_PARSER_H_
#define _REAL_TALK_LINKER_LINKER_CONFIG_PARSER_H_

#include <cstdint>
#include <stdexcept>
#include <string>

namespace real_talk {
namespace linker {

class LinkerConfig;
enum class LinkerCmd: uint8_t;

class LinkerConfigParser {
 public:
  class BadArgsError: public std::runtime_error {
   public:
    explicit BadArgsError(const std::string &msg): std::runtime_error(msg) {}
  };

  virtual ~LinkerConfigParser() {}

  /**
   * @throws real_talk::linker::LinkerConfigParser::BadArgsError
   */
  virtual void Parse(int argc,
                     const char *argv[],
                     LinkerConfig *config,
                     LinkerCmd *cmd) const = 0;
  virtual std::string GetHelp() const = 0;
};
}
}
#endif
