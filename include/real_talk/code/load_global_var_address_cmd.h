
#ifndef _REAL_TALK_CODE_LOAD_GLOBAL_VAR_ADDRESS_CMD_H_
#define _REAL_TALK_CODE_LOAD_GLOBAL_VAR_ADDRESS_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadGlobalVarAddressCmd: public Cmd {
 public:
  inline explicit LoadGlobalVarAddressCmd(uint32_t var_index) noexcept
      : var_index_(var_index) {}

  inline void SetVarIndex(uint32_t var_index) noexcept {var_index_ = var_index;}
  inline uint32_t GetVarIndex() const noexcept {return var_index_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "var_index=" << var_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadGlobalVarAddressCmd &rhs =
        static_cast<const LoadGlobalVarAddressCmd&>(cmd);
    return var_index_ == rhs.var_index_;
  }

  uint32_t var_index_;
};
}
}
#endif
