
#ifndef _REAL_TALK_CODE_CREATE_AND_INIT_LOCAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_AND_INIT_LOCAL_VAR_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateAndInitLocalVarCmd: public Cmd {
 protected:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class CreateAndInitLocalIntVarCmd: public CreateAndInitLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalIntVar(*this);
  }
};

class CreateAndInitLocalLongVarCmd: public CreateAndInitLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalLongVar(*this);
  }
};

class CreateAndInitLocalDoubleVarCmd: public CreateAndInitLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalDoubleVar(*this);
  }
};

class CreateAndInitLocalCharVarCmd: public CreateAndInitLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalCharVar(*this);
  }
};

class CreateAndInitLocalStringVarCmd: public CreateAndInitLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalStringVar(*this);
  }
};

class CreateAndInitLocalBoolVarCmd: public CreateAndInitLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalBoolVar(*this);
  }
};


class CreateAndInitLocalArrayVarCmd: public CreateAndInitLocalVarCmd {
 public:
  inline explicit CreateAndInitLocalArrayVarCmd(
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
    CreateAndInitLocalVarCmd::Print(stream);
    stream << "; dimensions_count=" << static_cast<int>(dimensions_count_);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const auto &rhs = static_cast<const CreateAndInitLocalArrayVarCmd&>(cmd);
    return CreateAndInitLocalVarCmd::IsEqual(cmd)
        && dimensions_count_ == rhs.dimensions_count_;
  }

  uint8_t dimensions_count_;
};

class CreateAndInitLocalIntArrayVarCmd: public CreateAndInitLocalArrayVarCmd {
 public:
  using CreateAndInitLocalArrayVarCmd::CreateAndInitLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalIntArrayVar(*this);
  }
};

class CreateAndInitLocalLongArrayVarCmd: public CreateAndInitLocalArrayVarCmd {
 public:
  using CreateAndInitLocalArrayVarCmd::CreateAndInitLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalLongArrayVar(*this);
  }
};

class CreateAndInitLocalDoubleArrayVarCmd
    : public CreateAndInitLocalArrayVarCmd {
 public:
  using CreateAndInitLocalArrayVarCmd::CreateAndInitLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalDoubleArrayVar(*this);
  }
};

class CreateAndInitLocalCharArrayVarCmd: public CreateAndInitLocalArrayVarCmd {
 public:
  using CreateAndInitLocalArrayVarCmd::CreateAndInitLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalCharArrayVar(*this);
  }
};

class CreateAndInitLocalStringArrayVarCmd
    : public CreateAndInitLocalArrayVarCmd {
 public:
  using CreateAndInitLocalArrayVarCmd::CreateAndInitLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalStringArrayVar(*this);
  }
};

class CreateAndInitLocalBoolArrayVarCmd: public CreateAndInitLocalArrayVarCmd {
 public:
  using CreateAndInitLocalArrayVarCmd::CreateAndInitLocalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalBoolArrayVar(*this);
  }
};
}
}
#endif
