
#ifndef _REAL_TALK_CODE_CMD_READER_H_
#define _REAL_TALK_CODE_CMD_READER_H_

#include <array>
#include <limits>

namespace real_talk {
namespace code {

class Cmd;
class Code;
class CreateArrayCmd;
class CreateAndInitArrayCmd;
class JumpCmd;
class LoadGlobalVarValueCmd;
class LoadLocalVarValueCmd;

class CmdReader {
 public:
  void SetCode(Code *code);
  Code *GetCode() const;
  const Cmd &GetNextCmd();

 private:
  void ReadCreateArrayCmd(CreateArrayCmd &cmd);
  void ReadJumpCmd(JumpCmd &cmd);
  void ReadCreateAndInitArrayCmd(CreateAndInitArrayCmd &cmd);
  void ReadLoadGlobalVarValueCmd(LoadGlobalVarValueCmd &cmd);
  void ReadLoadLocalVarValueCmd(LoadLocalVarValueCmd &cmd);

  Code *code_;
};
}
}
#endif
