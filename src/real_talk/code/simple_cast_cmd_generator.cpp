
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

void SimpleCastCmdGenerator::HandleCharToInt() {
  cmd_id_ = CmdId::kCastCharToInt;
}

void SimpleCastCmdGenerator::HandleCharToLong() {
  cmd_id_ = CmdId::kCastCharToLong;
}

void SimpleCastCmdGenerator::HandleCharToDouble() {
  cmd_id_ = CmdId::kCastCharToDouble;
}

void SimpleCastCmdGenerator::HandleCharToString() {
  cmd_id_ = CmdId::kCastCharToString;
}

void SimpleCastCmdGenerator::HandleIntToLong() {
  cmd_id_ = CmdId::kCastIntToLong;
}

void SimpleCastCmdGenerator::HandleIntToDouble() {
  cmd_id_ = CmdId::kCastIntToDouble;
}

void SimpleCastCmdGenerator::HandleLongToDouble() {
  cmd_id_ = CmdId::kCastLongToDouble;
}

void SimpleCastCmdGenerator::HandleFail() {assert(false);}
}
}
