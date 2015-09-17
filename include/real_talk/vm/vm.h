
#ifndef _REAL_TALK_VM_VM_H_
#define _REAL_TALK_VM_VM_H_

#include <vector>
#include <stdexcept>
#include "real_talk/vm/func_frame.h"

namespace real_talk {
namespace vm {

class DataStorage;

class VM {
 public:
  typedef std::vector<FuncFrame> FuncFrames;

  class ExecutionError: public std::runtime_error {
   public:
    using std::runtime_error::runtime_error;
  };

  virtual ~VM() {}

  /**
   * @throws real_talk::vm::VM::ExecutionError
   */
  virtual void Execute() = 0;

  virtual const DataStorage &GetGlobalVars() const noexcept = 0;
  virtual const DataStorage &GetLocalVars() const noexcept = 0;
  virtual const DataStorage &GetOperands() const noexcept = 0;
  virtual DataStorage &GetOperands() noexcept = 0;
  virtual const FuncFrames &GetFuncFrames() const noexcept = 0;
};
}
}
#endif
