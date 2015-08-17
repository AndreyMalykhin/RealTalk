
#ifndef _REAL_TALK_VM_VM_FACTORY_H_
#define _REAL_TALK_VM_VM_FACTORY_H_

#include <memory>
#include <vector>
#include "real_talk/vm/native_func_value.h"

namespace real_talk {
namespace code {

class Exe;
}

namespace vm {

class VM;

class VMFactory {
 public:
  virtual ~VMFactory() {}
  virtual std::unique_ptr<VM> Create(
      real_talk::code::Exe *exe,
      const std::vector<NativeFuncValue> &native_funcs) const = 0;
};
}
}
#endif
