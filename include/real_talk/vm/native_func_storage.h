
#ifndef _REAL_TALK_VM_NATIVE_FUNC_STORAGE_H_
#define _REAL_TALK_VM_NATIVE_FUNC_STORAGE_H_

#include <unordered_map>
#include <string>
#include "real_talk/vm/native_func.h"

namespace real_talk {
namespace vm {

class NativeFuncStorage {
 public:
  virtual ~NativeFuncStorage() {}
  virtual const std::unordered_map<std::string, NativeFunc> &GetAll() const = 0;
};
}
}
#endif
