
#ifndef _REAL_TALK_CODE_CMD_READER_H_
#define _REAL_TALK_CODE_CMD_READER_H_

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
  void SetCode(Code *code) noexcept;
  Code *GetCode() const noexcept;
  const Cmd &GetNextCmd() noexcept;

 private:
  void ReadCreateArrayCmd(CreateArrayCmd &cmd) noexcept;
  void ReadJumpCmd(JumpCmd &cmd) noexcept;
  void ReadCreateAndInitArrayCmd(CreateAndInitArrayCmd &cmd) noexcept;
  void ReadLoadGlobalVarValueCmd(LoadGlobalVarValueCmd &cmd) noexcept;
  void ReadLoadLocalVarValueCmd(LoadLocalVarValueCmd &cmd) noexcept;

  Code *code_;
};
}
}
#endif
