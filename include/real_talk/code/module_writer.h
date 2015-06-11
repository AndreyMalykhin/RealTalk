
#ifndef _REAL_TALK_CODE_MODULE_WRITER_H_
#define _REAL_TALK_CODE_MODULE_WRITER_H_

namespace real_talk {
namespace code {

class Module;
class Code;

class ModuleWriter {
 public:
  virtual ~ModuleWriter() {}

  /**
   * @throws real_talk::code::CodeSizeOverflowError
   */
  virtual void Write(const Module &module, Code *code) const = 0;
};
}
}
#endif
