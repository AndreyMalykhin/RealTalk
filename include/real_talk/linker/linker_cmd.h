
#ifndef _REAL_TALK_LINKER_LINKER_CMD_H_
#define _REAL_TALK_LINKER_LINKER_CMD_H_

#include <cstdint>

namespace real_talk {
namespace linker {

enum class LinkerCmd: uint8_t {
  kLink = UINT8_C(1),
  kHelp = UINT8_C(2)
};
}
}
#endif
