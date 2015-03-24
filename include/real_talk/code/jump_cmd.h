
#ifndef _REAL_TALK_CODE_JUMP_CMD_H_
#define _REAL_TALK_CODE_JUMP_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class JumpIfNotCmd: public Cmd {
 public:
  explicit JumpIfNotCmd(uint32_t address): address_(address) {}
  void SetAddress(uint32_t address) {address_ = address;}
  uint32_t GetAddress() const {return address_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "address=" << address_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const JumpIfNotCmd &rhs = static_cast<const JumpIfNotCmd&>(cmd);
    return address_ == rhs.address_;
  }

  uint32_t address_;
};
}
}
#endif
