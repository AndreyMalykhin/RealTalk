
#ifndef _REAL_TALK_CODE_CAST_CMD_GENERATOR_H_
#define _REAL_TALK_CODE_CAST_CMD_GENERATOR_H_

#include <cstdint>

namespace real_talk {
namespace semantic {

class DataType;
}

namespace code {

class Code;
enum class CmdId: uint8_t;

class CastCmdGenerator {
 public:
  virtual ~CastCmdGenerator() {}
  virtual CmdId Generate(
      const real_talk::semantic::DataType &dest_data_type,
      const real_talk::semantic::DataType &src_data_type) const = 0;
};
}
}

#endif
