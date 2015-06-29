
#ifndef _REAL_TALK_LINKER_OUTPUT_FILE_TYPE_H_
#define _REAL_TALK_LINKER_OUTPUT_FILE_TYPE_H_

#include <cstdint>
#include <iostream>

namespace real_talk {
namespace linker {

enum class OutputFileType: uint8_t {
  kExe = UINT8_C(1),
  kLib = UINT8_C(2)
};

inline std::ostream &operator<<(
    std::ostream &stream, OutputFileType output_file_type) {
  return stream << (output_file_type == OutputFileType::kExe ? "exe" : "lib");
}
}
}
#endif
