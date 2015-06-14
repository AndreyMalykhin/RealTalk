
#ifndef _REAL_TALK_LINKER_LINKER_CONFIG_PARSER_H_
#define _REAL_TALK_LINKER_LINKER_CONFIG_PARSER_H_

#include <string>

namespace real_talk {
namespace linker {

class LinkerConfig;
enum class LinkerCmd: uint8_t;

class LinkerConfigParser {
 public:
  virtual ~LinkerConfigParser() {}
  virtual void Parse(int argc,
                     const char *argv[],
                     LinkerConfig *config,
                     LinkerCmd *cmd) const = 0;
  virtual std::string GetHelp() const = 0;
};
}
}
#endif
