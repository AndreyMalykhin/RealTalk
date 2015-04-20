
#include <cassert>
#include "real_talk/code/cmd.h"
#include "real_talk/code/simple_cast_cmd_generator.h"

using real_talk::semantic::DataType;

namespace real_talk {
namespace code {

CmdId SimpleCastCmdGenerator::Generate(
      const DataType &dest_data_type, const DataType &src_data_type) const {
  Handle(dest_data_type, src_data_type, Direction::kRightToLeft);
  return cmd_id_;
}

void SimpleCastCmdGenerator::HandleCharToInt() const {
  cmd_id_ = CmdId::kCastCharToInt;
}

void SimpleCastCmdGenerator::HandleCharToLong() const {
  cmd_id_ = CmdId::kCastCharToLong;
}

void SimpleCastCmdGenerator::HandleCharToDouble() const {
  cmd_id_ = CmdId::kCastCharToDouble;
}

void SimpleCastCmdGenerator::HandleCharToString() const {
  cmd_id_ = CmdId::kCastCharToString;
}

void SimpleCastCmdGenerator::HandleIntToLong() const {
  cmd_id_ = CmdId::kCastIntToLong;
}

void SimpleCastCmdGenerator::HandleIntToDouble() const {
  cmd_id_ = CmdId::kCastIntToDouble;
}

void SimpleCastCmdGenerator::HandleLongToDouble() const {
  cmd_id_ = CmdId::kCastLongToDouble;
}

void SimpleCastCmdGenerator::HandleFail() const {assert(false);}
}
}
