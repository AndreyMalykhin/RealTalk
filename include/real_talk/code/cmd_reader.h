
#ifndef _REAL_TALK_CODE_CMD_READER_H_
#define _REAL_TALK_CODE_CMD_READER_H_

#include <array>
#include <limits>

namespace real_talk {
namespace code {

class Cmd;
class Code;

class CmdReader {
 public:
  void SetCode(Code *code);
  const Cmd &GetNextCmd();

 private:
  typedef const Cmd& (CmdReader::*Reader)();
  typedef std::array<Reader, std::numeric_limits<uint8_t>::max()> Readers;

  static const Readers InitReaders();
  const Cmd &ReadEndMain();
  const Cmd &ReadEndFuncs();
  const Cmd &ReadCreateGlobalInt();

  static const Readers kReaders;
  Code *code_;
};
}
}
#endif
