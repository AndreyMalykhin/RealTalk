
#ifndef _REAL_TALK_LINKER_OUTPUT_FILE_TYPE_H_
#define _REAL_TALK_LINKER_OUTPUT_FILE_TYPE_H_

#include <cstdint>

namespace real_talk {
namespace linker {

enum class OutputFileType: uint8_t {
  kExe = UINT8_C(1),
  kLib = UINT8_C(2)
};
}
}
#endif
