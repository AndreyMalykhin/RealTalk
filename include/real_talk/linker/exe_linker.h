
#ifndef _REAL_TALK_LINKER_EXE_LINKER_H_
#define _REAL_TALK_LINKER_EXE_LINKER_H_

#include "real_talk/linker/linker.h"

namespace real_talk {
namespace linker {

class ExeLinker: public Linker {
 public:
  virtual std::unique_ptr<real_talk::code::CodeContainer> Link(
      const Linker::Modules &modules,
      uint32_t output_code_version) const override;
};
}
}
#endif
