
#ifndef _REAL_TALK_VM_SIMPLE_VM_H_
#define _REAL_TALK_VM_SIMPLE_VM_H_

#include <memory>
#include <vector>
#include "real_talk/vm/native_func_value.h"
#include "real_talk/vm/vm.h"

namespace real_talk {
namespace code {

class Exe;
}

namespace vm {

class SimpleVM: public VM {
 public:
  SimpleVM(real_talk::code::Exe *exe,
           const std::vector<NativeFuncValue> &native_funcs);
  ~SimpleVM();
  virtual void Execute() override;
  virtual const DataStorage &GetGlobalVars() const noexcept override;
  virtual const DataStorage &GetLocalVars() const noexcept override;
  virtual const DataStorage &GetOperands() const noexcept override;
  virtual DataStorage &GetOperands() noexcept override;
  virtual const FuncFrames &GetFuncFrames() const noexcept override;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
#endif
