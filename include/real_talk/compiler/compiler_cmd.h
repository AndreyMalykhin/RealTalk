
#ifndef _REAL_TALK_COMPILER_COMPILER_CMD_H_
#define _REAL_TALK_COMPILER_COMPILER_CMD_H_

#include <cstdint>

namespace real_talk {
namespace compiler {

enum class CompilerCmd: uint8_t {
  kCompile = UINT8_C(1),
  kHelp = UINT8_C(2)
};
}
}
#endif
