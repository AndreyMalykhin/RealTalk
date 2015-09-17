
#ifndef _REAL_TALK_VM_SIMPLE_VM_CONFIG_PARSER_H_
#define _REAL_TALK_VM_SIMPLE_VM_CONFIG_PARSER_H_

#include <string>
#include "real_talk/vm/vm_config_parser.h"

namespace real_talk {
namespace vm {

class SimpleVMConfigParser: public VMConfigParser {
 public:
  virtual std::string GetHelp() const override;
  virtual void Parse(int argc,
                     const char *argv[],
                     VMConfig *config,
                     VMCmd *cmd) const override;
};
}
}
#endif
