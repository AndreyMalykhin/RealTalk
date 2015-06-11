
#ifndef _REAL_TALK_CODE_SIMPLE_MODULE_WRITER_H_
#define _REAL_TALK_CODE_SIMPLE_MODULE_WRITER_H_

#include "real_talk/code/module_writer.h"

namespace real_talk {
namespace code {

class SimpleModuleWriter: public ModuleWriter {
 public:
  virtual void Write(const Module &module, Code *code) const override;
};
}
}

#endif
