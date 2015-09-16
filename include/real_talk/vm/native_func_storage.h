
#ifndef _REAL_TALK_VM_NATIVE_FUNC_STORAGE_H_
#define _REAL_TALK_VM_NATIVE_FUNC_STORAGE_H_

#include <unordered_map>
#include <string>
#include "real_talk/vm/native_func_value.h"

namespace real_talk {
namespace vm {

class NativeFuncStorage {
 public:
  typedef std::unordered_map<std::string, NativeFuncValue> NativeFuncsMap;

  virtual ~NativeFuncStorage() {}
  virtual const NativeFuncsMap &GetAll() const = 0;
};
}
}
#endif
