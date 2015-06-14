
#ifndef _REAL_TALK_CODE_SIMPLE_MODULE_READER_H_
#define _REAL_TALK_CODE_SIMPLE_MODULE_READER_H_

#include "real_talk/code/module_reader.h"

namespace real_talk {
namespace code {

class SimpleModuleReader: public ModuleReader {
 public:
  virtual std::unique_ptr<Module> ReadFromCode(Code *code) const override;
  virtual std::unique_ptr<Module> ReadFromStream(
      std::istream *code_stream) const override;
};
}
}
#endif
