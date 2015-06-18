
#ifndef _REAL_TALK_CODE_CODE_CONTAINER_WRITER_H_
#define _REAL_TALK_CODE_CODE_CONTAINER_WRITER_H_

#include "real_talk/code/module_writer.h"

namespace real_talk {
namespace code {

class CodeContainer;
class Code;

class CodeContainerWriter: public ModuleWriter {
 public:
  virtual ~CodeContainerWriter() {}

  /**
   * @throws real_talk::code::Code::CodeSizeOverflowError
   */
  virtual void Write(const CodeContainer &container, Code *code) const = 0;

  using ModuleWriter::Write;
};
}
}
#endif
