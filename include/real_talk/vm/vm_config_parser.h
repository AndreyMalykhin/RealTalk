
#ifndef _REAL_TALK_VM_VM_CONFIG_PARSER_H_
#define _REAL_TALK_VM_VM_CONFIG_PARSER_H_

namespace real_talk {
namespace vm {

class VMConfig;
enum class VMCmd: uint8_t;

class VMConfigParser {
 public:
  virtual ~VMConfigParser() {}
  virtual void Parse(
      int argc, const char *argv[], VMConfig *config, VMCmd *cmd) const = 0;
};
}
}
#endif
