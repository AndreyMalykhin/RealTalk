
#ifndef _REAL_TALK_VM_VM_CONFIG_PARSER_H_
#define _REAL_TALK_VM_VM_CONFIG_PARSER_H_

#include <stdexcept>
#include <string>

namespace real_talk {
namespace vm {

class VMConfig;
enum class VMCmd: uint8_t;

class VMConfigParser {
 public:
  class BadArgsError: public std::runtime_error {
   public:
    explicit BadArgsError(const std::string &msg): std::runtime_error(msg) {}
  };

  virtual ~VMConfigParser() {}
  virtual std::string GetHelp() const = 0;
  virtual void Parse(
      int argc, const char *argv[], VMConfig *config, VMCmd *cmd) const = 0;
};
}
}
#endif
