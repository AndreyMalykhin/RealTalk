
#ifndef _REAL_TALK_CODE_CMD_READER_H_
#define _REAL_TALK_CODE_CMD_READER_H_

#include <array>
#include <limits>

namespace real_talk {
namespace code {

class Cmd;
class Code;
class CreateGlobalVarCmd;

class CmdReader {
 public:
  void SetCode(Code *code);
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

  static const Readers kReaders;
  Code *code_;
};
}
}
#endif
