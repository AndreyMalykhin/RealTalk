
#ifndef _REAL_TALK_VM_SIMPLE_VM_H_
#define _REAL_TALK_VM_SIMPLE_VM_H_

#include <memory>
#include <vector>
#include "real_talk/vm/native_func.h"
#include "real_talk/vm/func_frame.h"
#include "real_talk/vm/vm.h"

namespace real_talk {
namespace code {

class Exe;
}

namespace vm {

class DataStorage;

class SimpleVM: public VM {
 public:
  typedef std::vector<FuncFrame> FuncFrames;

  SimpleVM(real_talk::code::Exe *exe,
           const std::vector<NativeFunc> &native_funcs);
  ~SimpleVM();
  virtual void Execute() const override;
  const DataStorage &GetGlobalVars() const;
  const DataStorage &GetLocalVars() const;
  const DataStorage &GetOperands() const;
  const FuncFrames &GetFuncFrames() const;

 private:
  class Impl;
  std::unique_ptr<Impl> impl_;
};
}
}
#endif
