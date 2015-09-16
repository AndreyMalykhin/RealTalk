
#ifndef _REAL_TALK_VM_SIMPLE_NATIVE_FUNC_STORAGE_H_
#define _REAL_TALK_VM_SIMPLE_NATIVE_FUNC_STORAGE_H_

#include <string>
#include "real_talk/vm/native_func_storage.h"

namespace real_talk {
namespace vm {

class SimpleNativeFuncStorage: public NativeFuncStorage {
 public:
  virtual const NativeFuncsMap &GetAll() const override;
};
}
}
#endif
