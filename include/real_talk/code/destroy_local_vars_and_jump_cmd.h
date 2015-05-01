
#ifndef _REAL_TALK_CODE_DESTROY_LOCAL_VARS_AND_JUMP_CMD_H_
#define _REAL_TALK_CODE_DESTROY_LOCAL_VARS_AND_JUMP_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class DestroyLocalVarsAndJumpCmd: public Cmd {
 public:
  inline explicit DestroyLocalVarsAndJumpCmd(
      uint32_t vars_count, int32_t offset) noexcept
      : vars_count_(vars_count), offset_(offset) {assert(vars_count > 0);}

  inline void SetVarsCount(uint32_t count) noexcept {vars_count_ = count;}
  inline uint32_t GetVarsCount() const noexcept {return vars_count_;}
  inline void SetOffset(int32_t offset) noexcept {offset_ = offset;}
  inline int32_t GetOffset() const noexcept {return offset_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "vars_count=" << vars_count_ << "; offset=" << offset_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const DestroyLocalVarsAndJumpCmd &rhs =
        static_cast<const DestroyLocalVarsAndJumpCmd&>(cmd);
    return vars_count_ == rhs.vars_count_ && offset_ == rhs.offset_;
  }

  uint32_t vars_count_;
  int32_t offset_;
};
}
}
#endif
