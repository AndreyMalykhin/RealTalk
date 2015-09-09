
#ifndef _REAL_TALK_VM_VM_H_
#define _REAL_TALK_VM_VM_H_

#include <vector>
#include "real_talk/vm/func_frame.h"

namespace real_talk {
namespace vm {

class DataStorage;

class VM {
 public:
  typedef std::vector<FuncFrame> FuncFrames;

  virtual ~VM() {}

  /**
   * @throws real_talk::vm::DataStorage::OverflowError
   */
  virtual void Execute() = 0;

  virtual const DataStorage &GetGlobalVars() const = 0;
  virtual const DataStorage &GetLocalVars() const = 0;
  virtual const DataStorage &GetOperands() const = 0;
  virtual const FuncFrames &GetFuncFrames() const = 0;
};
}
}
#endif
