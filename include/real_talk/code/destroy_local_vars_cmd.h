
#ifndef _REAL_TALK_CODE_DESTROY_LOCAL_VARS_CMD_H_
#define _REAL_TALK_CODE_DESTROY_LOCAL_VARS_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class DestroyLocalVarsCmd: public Cmd {
 public:
  inline explicit DestroyLocalVarsCmd(uint32_t vars_count) noexcept
      : vars_count_(vars_count) {assert(vars_count != 0);}

  inline void SetVarsCount(uint32_t count) noexcept {vars_count_ = count;}
  inline uint32_t GetVarsCount() const noexcept {return vars_count_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "vars_count=" << vars_count_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const DestroyLocalVarsCmd &rhs =
        static_cast<const DestroyLocalVarsCmd&>(cmd);
    return vars_count_ == rhs.vars_count_;
  }

  uint32_t vars_count_;
};
}
}
#endif
