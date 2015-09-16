
#ifndef _REAL_TALK_VM_SIMPLE_VM_FACTORY_H_
#define _REAL_TALK_VM_SIMPLE_VM_FACTORY_H_

#include <vector>
#include "real_talk/vm/vm_factory.h"

namespace real_talk {
namespace vm {

class SimpleVMFactory: public VMFactory {
 public:
  virtual std::unique_ptr<VM> Create(
      real_talk::code::Exe *exe,
      const std::vector<NativeFuncValue> &native_funcs) const override;
};
}
}
#endif
