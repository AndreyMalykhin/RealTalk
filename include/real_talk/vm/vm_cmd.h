
#ifndef _REAL_TALK_VM_VM_CMD_H_
#define _REAL_TALK_VM_VM_CMD_H_

#include <cstdint>

namespace real_talk {
namespace vm {

enum class VMCmd: uint8_t {
  kExecute = UINT8_C(1),
  kHelp = UINT8_C(2)
};
}
}
#endif
