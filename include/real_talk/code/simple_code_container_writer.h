
#ifndef _REAL_TALK_CODE_SIMPLE_CODE_CONTAINER_WRITER_H_
#define _REAL_TALK_CODE_SIMPLE_CODE_CONTAINER_WRITER_H_

#include "real_talk/code/code_container_visitor.h"
#include "real_talk/code/code_container_writer.h"

namespace real_talk {
namespace code {

class SimpleCodeContainerWriter: public CodeContainerWriter,
                                 private CodeContainerVisitor {
 public:
  virtual void Write(
      const CodeContainer &container, Code *output_code) const override;
  virtual void Write(const Module &module, Code *output_code) const override;
  virtual void Write(const Exe &exe, Code *output_code) const override;

 private:
  virtual void VisitModule(const Module &module) const override;
  virtual void VisitExe(const Exe &exe) const override;

  mutable Code *output_code_;
};
}
}
#endif
