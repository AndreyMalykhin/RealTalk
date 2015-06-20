
#ifndef _REAL_TALK_CODE_EXE_WRITER_H_
#define _REAL_TALK_CODE_EXE_WRITER_H_

namespace real_talk {
namespace code {

class Exe;
class Code;

class ExeWriter {
 public:
  virtual ~ExeWriter() {}

  /**
   * @throws real_talk::code::CodeSizeOverflowError
   */
  virtual void Write(const Exe &exe, Code *code) const = 0;
};
}
}
#endif
