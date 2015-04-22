
#ifndef _REAL_TALK_CODE_LOAD_NATIVE_FUNC_ADDRESS_CMD_H_
#define _REAL_TALK_CODE_LOAD_NATIVE_FUNC_ADDRESS_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadNativeFuncAddressCmd: public Cmd {
 public:
  inline explicit LoadNativeFuncAddressCmd(uint32_t func_index) noexcept
      : func_index_(func_index) {}

  inline void SetFuncIndex(uint32_t func_index) noexcept {
    func_index_ = func_index;
  }

  inline uint32_t GetFuncIndex() const noexcept {return func_index_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "func_index=" << func_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadNativeFuncAddressCmd &rhs =
        static_cast<const LoadNativeFuncAddressCmd&>(cmd);
    return func_index_ == rhs.func_index_;
  }

  uint32_t func_index_;
};
}
}
#endif
