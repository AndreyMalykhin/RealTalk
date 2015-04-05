
#ifndef _REAL_TALK_CODE_CMD_READER_H_
#define _REAL_TALK_CODE_CMD_READER_H_

#include <array>
#include <limits>

namespace real_talk {
namespace code {

class Cmd;
class Code;
class CreateGlobalVarCmd;
class CreateAndInitGlobalVarCmd;
class CreateArrayCmd;
class CreateAndInitArrayCmd;
class JumpCmd;
class LoadGlobalVarValueCmd;

class CmdReader {
 public:
  void SetCode(Code *code);
  Code *GetCode() const;
  const Cmd &GetNextCmd();

 private:
  void ReadCreateGlobalVarCmd(CreateGlobalVarCmd &cmd);
  void ReadCreateAndInitGlobalVarCmd(CreateAndInitGlobalVarCmd &cmd);
  void ReadCreateArrayCmd(CreateArrayCmd &cmd);
  void ReadJumpCmd(JumpCmd &cmd);
  void ReadCreateAndInitArrayCmd(CreateAndInitArrayCmd &cmd);
  void ReadLoadGlobalVarValueCmd(LoadGlobalVarValueCmd &cmd);

  Code *code_;
};
}
}
#endif
