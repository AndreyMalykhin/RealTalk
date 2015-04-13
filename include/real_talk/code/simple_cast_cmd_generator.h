
#ifndef _REAL_TALK_CODE_SIMPLE_CAST_CMD_GENERATOR_H_
#define _REAL_TALK_CODE_SIMPLE_CAST_CMD_GENERATOR_H_

#include "real_talk/code/cast_cmd_generator.h"

namespace real_talk {
namespace code {

class SimpleCastCmdGenerator: public CastCmdGenerator {
 public:
  virtual CmdId Generate(
      const real_talk::semantic::DataType &dest_data_type,
      const real_talk::semantic::DataType &src_data_type) const override;
};
}
}

#endif
