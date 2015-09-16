
#ifndef _REAL_TALK_VM_NATIVE_FUNC_LINKER_H_
#define _REAL_TALK_VM_NATIVE_FUNC_LINKER_H_

#include <stdexcept>
#include <string>
#include <vector>
#include "real_talk/vm/native_func_value.h"

namespace real_talk {
namespace code {

class Exe;
}

namespace vm {

class NativeFuncLinker {
 public:
  class MissingFuncError: public std::runtime_error {
   public:
    MissingFuncError(const std::string &func_id, const std::string &msg)
        : std::runtime_error(msg), func_id_(func_id) {}
    const std::string &GetFuncId() const {return func_id_;}

   private:
    std::string func_id_;
  };

  virtual ~NativeFuncLinker() {}

  /**
   * @throws real_talk::vm::NativeFuncLinker::MissingFuncError
   */
  virtual void Link(
      const NativeFuncStorage::NativeFuncsMap &available_funcs,
      std::vector<NativeFuncValue> *output_used_funcs,
      real_talk::code::Exe *exe) const = 0;
};
}
}
#endif
