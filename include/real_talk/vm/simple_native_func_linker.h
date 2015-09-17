
#ifndef _REAL_TALK_VM_SIMPLE_NATIVE_FUNC_LINKER_H_
#define _REAL_TALK_VM_SIMPLE_NATIVE_FUNC_LINKER_H_

#include <string>
#include <vector>
#include "real_talk/vm/native_func_linker.h"

namespace real_talk {
namespace vm {

class SimpleNativeFuncLinker: public NativeFuncLinker {
 public:
  virtual void Link(
      const NativeFuncStorage::NativeFuncsMap &available_funcs,
      std::vector<NativeFuncValue> *output_used_funcs,
      real_talk::code::Exe *exe) const override;
};
}
}
#endif
