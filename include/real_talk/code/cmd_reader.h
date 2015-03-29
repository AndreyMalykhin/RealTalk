
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

class CmdReader {
 public:
  void SetCode(Code *code);
  Code *GetCode() const;
  const Cmd &GetNextCmd();

 private:
  typedef const Cmd& (CmdReader::*Reader)();
  typedef std::array<Reader, std::numeric_limits<uint8_t>::max()> Readers;

  static const Readers InitReaders();
  const Cmd &ReadEndMainCmd();
  const Cmd &ReadEndFuncsCmd();
  const Cmd &ReadCreateGlobalVarCmd(CreateGlobalVarCmd &cmd);
  const Cmd &ReadCreateGlobalIntVarCmd();
  const Cmd &ReadCreateGlobalArrayVarCmd();
  const Cmd &ReadCreateGlobalLongVarCmd();
  const Cmd &ReadCreateGlobalDoubleVarCmd();
  const Cmd &ReadCreateGlobalCharVarCmd();
  const Cmd &ReadCreateGlobalStringVarCmd();
  const Cmd &ReadCreateGlobalBoolVarCmd();
  const Cmd &ReadCreateLocalIntVarCmd();
  const Cmd &ReadCreateLocalArrayVarCmd();
  const Cmd &ReadCreateLocalLongVarCmd();
  const Cmd &ReadCreateLocalDoubleVarCmd();
  const Cmd &ReadCreateLocalCharVarCmd();
  const Cmd &ReadCreateLocalStringVarCmd();
  const Cmd &ReadCreateLocalBoolVarCmd();
  const Cmd &ReadUnloadCmd();
  const Cmd &ReadLoadIntValueCmd();
  const Cmd &ReadLoadLongValueCmd();
  const Cmd &ReadLoadBoolValueCmd();
  const Cmd &ReadLoadCharValueCmd();
  const Cmd &ReadLoadStringValueCmd();
  const Cmd &ReadLoadDoubleValueCmd();
  const Cmd &ReadCreateAndInitGlobalVarCmd(CreateAndInitGlobalVarCmd &cmd);
  const Cmd &ReadCreateAndInitGlobalIntVarCmd();
  const Cmd &ReadCreateAndInitGlobalArrayVarCmd();
  const Cmd &ReadCreateAndInitGlobalLongVarCmd();
  const Cmd &ReadCreateAndInitGlobalDoubleVarCmd();
  const Cmd &ReadCreateAndInitGlobalCharVarCmd();
  const Cmd &ReadCreateAndInitGlobalStringVarCmd();
  const Cmd &ReadCreateAndInitGlobalBoolVarCmd();
  const Cmd &ReadCreateAndInitLocalIntVarCmd();
  const Cmd &ReadCreateAndInitLocalArrayVarCmd();
  const Cmd &ReadCreateAndInitLocalLongVarCmd();
  const Cmd &ReadCreateAndInitLocalDoubleVarCmd();
  const Cmd &ReadCreateAndInitLocalCharVarCmd();
  const Cmd &ReadCreateAndInitLocalStringVarCmd();
  const Cmd &ReadCreateAndInitLocalBoolVarCmd();
  const Cmd &ReadCreateArrayCmd(CreateArrayCmd &cmd);
  const Cmd &ReadCreateIntArrayCmd();
  const Cmd &ReadCreateLongArrayCmd();
  const Cmd &ReadCreateDoubleArrayCmd();
  const Cmd &ReadCreateBoolArrayCmd();
  const Cmd &ReadCreateCharArrayCmd();
  const Cmd &ReadCreateStringArrayCmd();
  const Cmd &ReadJumpCmd(JumpCmd &cmd);
  const Cmd &ReadJumpIfNotCmd();
  const Cmd &ReadDirectJumpCmd();
  const Cmd &ReadDestroyLocalVarsAndJumpCmd();
  const Cmd &ReadDestroyLocalVarsCmd();
  const Cmd &ReadCreateAndInitArrayCmd(CreateAndInitArrayCmd &cmd);
  const Cmd &ReadCreateAndInitIntArrayCmd();
  const Cmd &ReadCreateAndInitLongArrayCmd();
  const Cmd &ReadCreateAndInitDoubleArrayCmd();
  const Cmd &ReadCreateAndInitBoolArrayCmd();
  const Cmd &ReadCreateAndInitCharArrayCmd();
  const Cmd &ReadCreateAndInitStringArrayCmd();

  static const Readers kReaders;
  Code *code_;
};
}
}
#endif
