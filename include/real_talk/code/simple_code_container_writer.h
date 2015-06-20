
#ifndef _REAL_TALK_CODE_SIMPLE_CODE_CONTAINER_WRITER_H_
#define _REAL_TALK_CODE_SIMPLE_CODE_CONTAINER_WRITER_H_

#include "real_talk/code/code_container_writer.h"

namespace real_talk {
namespace code {

class SimpleCodeContainerWriter: public CodeContainerWriter {
 public:
  virtual void Write(const CodeContainer &container, Code *code) const override;
  virtual void Write(const Module &module, Code *code) const override;
  virtual void Write(const Exe &exe, Code *code) const override;
};
}
}
#endif
