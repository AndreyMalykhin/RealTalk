
#ifndef _REAL_TALK_CODE_JUMP_CMD_H_
#define _REAL_TALK_CODE_JUMP_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class JumpCmd: public Cmd {
 public:
  inline explicit JumpCmd(uint32_t address) noexcept: address_(address) {}
  inline void SetAddress(uint32_t address) noexcept {address_ = address;}
  inline uint32_t GetAddress() const noexcept {return address_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "address=" << address_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const JumpCmd &rhs = static_cast<const JumpCmd&>(cmd);
    return address_ == rhs.address_;
  }

  uint32_t address_;
};

class JumpIfNotCmd: public JumpCmd {
 public:
  inline explicit JumpIfNotCmd(uint32_t address) noexcept: JumpCmd(address) {}
};

class DirectJumpCmd: public JumpCmd {
 public:
  inline explicit DirectJumpCmd(uint32_t address) noexcept: JumpCmd(address) {}
};
}
}
#endif
