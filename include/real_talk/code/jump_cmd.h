
#ifndef _REAL_TALK_CODE_JUMP_CMD_H_
#define _REAL_TALK_CODE_JUMP_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class JumpCmd: public Cmd {
 public:
  inline explicit JumpCmd(int32_t offset) noexcept: offset_(offset) {}
  inline void SetOffset(int32_t offset) noexcept {offset_ = offset;}
  inline int32_t GetOffset() const noexcept {return offset_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "offset=" << offset_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const JumpCmd &rhs = static_cast<const JumpCmd&>(cmd);
    return offset_ == rhs.offset_;
  }

  int32_t offset_;
};

class JumpIfNotCmd: public JumpCmd {
 public:
  using JumpCmd::JumpCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitJumpIfNot(*this);
  }
};

class DirectJumpCmd: public JumpCmd {
 public:
  using JumpCmd::JumpCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDirectJump(*this);
  }
};

class ImplicitJumpIfNotCmd: public JumpCmd {
 public:
  using JumpCmd::JumpCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitImplicitJumpIfNot(*this);
  }
};

class ImplicitJumpIfCmd: public JumpCmd {
 public:
  using JumpCmd::JumpCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitImplicitJumpIf(*this);
  }
};
}
}
#endif
