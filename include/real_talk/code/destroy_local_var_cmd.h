
#ifndef _REAL_TALK_CODE_DESTROY_LOCAL_VAR_CMD_H_
#define _REAL_TALK_CODE_DESTROY_LOCAL_VAR_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class DestroyLocalVarCmd: public Cmd {
 protected:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class DestroyLocalIntVarCmd: public DestroyLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalIntVar(*this);
  }
};

class DestroyLocalLongVarCmd: public DestroyLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalLongVar(*this);
  }
};

class DestroyLocalDoubleVarCmd: public DestroyLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalDoubleVar(*this);
  }
};

class DestroyLocalCharVarCmd: public DestroyLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalCharVar(*this);
  }
};

class DestroyLocalStringVarCmd: public DestroyLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalStringVar(*this);
  }
};

class DestroyLocalBoolVarCmd: public DestroyLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalBoolVar(*this);
  }
};

class DestroyLocalArrayVarCmd: public DestroyLocalVarCmd {
 public:
  inline explicit DestroyLocalArrayVarCmd(
      uint8_t dimensions_count) noexcept
      : dimensions_count_(dimensions_count) {
        assert(dimensions_count != UINT8_C(0));
      }

  inline uint8_t GetDimensionsCount() const noexcept {return dimensions_count_;}

  inline void SetDimensionsCount(uint8_t count) noexcept {
    dimensions_count_ = count;
  }

 private:
  virtual void Print(std::ostream &stream) const override {
    DestroyLocalVarCmd::Print(stream);
    stream << "; dimensions_count=" << static_cast<int>(dimensions_count_);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const auto &rhs = static_cast<const DestroyLocalArrayVarCmd&>(cmd);
    return DestroyLocalVarCmd::IsEqual(cmd)
        && dimensions_count_ == rhs.dimensions_count_;
  }

  uint8_t dimensions_count_;
};

class DestroyLocalIntArrayVarCmd: public DestroyLocalArrayVarCmd {
 public:
  using DestroyLocalArrayVarCmd::DestroyLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalIntArrayVar(*this);
  }
};

class DestroyLocalLongArrayVarCmd: public DestroyLocalArrayVarCmd {
 public:
  using DestroyLocalArrayVarCmd::DestroyLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalLongArrayVar(*this);
  }
};

class DestroyLocalDoubleArrayVarCmd: public DestroyLocalArrayVarCmd {
 public:
  using DestroyLocalArrayVarCmd::DestroyLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalDoubleArrayVar(*this);
  }
};

class DestroyLocalCharArrayVarCmd: public DestroyLocalArrayVarCmd {
 public:
  using DestroyLocalArrayVarCmd::DestroyLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalCharArrayVar(*this);
  }
};

class DestroyLocalStringArrayVarCmd: public DestroyLocalArrayVarCmd {
 public:
  using DestroyLocalArrayVarCmd::DestroyLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalStringArrayVar(*this);
  }
};

class DestroyLocalBoolArrayVarCmd: public DestroyLocalArrayVarCmd {
 public:
  using DestroyLocalArrayVarCmd::DestroyLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalBoolArrayVar(*this);
  }
};
}
}
#endif
