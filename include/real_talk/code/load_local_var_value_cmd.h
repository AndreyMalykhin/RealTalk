
#ifndef _REAL_TALK_CODE_LOAD_LOCAL_VAR_VALUE_CMD_H_
#define _REAL_TALK_CODE_LOAD_LOCAL_VAR_VALUE_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadLocalVarValueCmd: public Cmd {
 public:
  inline explicit LoadLocalVarValueCmd(uint32_t var_index) noexcept
      : var_index_(var_index) {}

  inline void SetVarIndex(uint32_t var_index) noexcept {var_index_ = var_index;}
  inline uint32_t GetVarIndex() const noexcept {return var_index_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "var_index=" << var_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadLocalVarValueCmd &rhs =
        static_cast<const LoadLocalVarValueCmd&>(cmd);
    return var_index_ == rhs.var_index_;
  }

  uint32_t var_index_;
};

class LoadLocalIntVarValueCmd: public LoadLocalVarValueCmd {
 public:
  using LoadLocalVarValueCmd::LoadLocalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadLocalIntVarValue(*this);
  }
};

class LoadLocalArrayVarValueCmd: public LoadLocalVarValueCmd {
 public:
  using LoadLocalVarValueCmd::LoadLocalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadLocalArrayVarValue(*this);
  }
};

class LoadLocalLongVarValueCmd: public LoadLocalVarValueCmd {
 public:
  using LoadLocalVarValueCmd::LoadLocalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadLocalLongVarValue(*this);
  }
};

class LoadLocalDoubleVarValueCmd: public LoadLocalVarValueCmd {
 public:
  using LoadLocalVarValueCmd::LoadLocalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadLocalDoubleVarValue(*this);
  }
};

class LoadLocalCharVarValueCmd: public LoadLocalVarValueCmd {
 public:
  using LoadLocalVarValueCmd::LoadLocalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadLocalCharVarValue(*this);
  }
};

class LoadLocalStringVarValueCmd: public LoadLocalVarValueCmd {
 public:
  using LoadLocalVarValueCmd::LoadLocalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadLocalStringVarValue(*this);
  }
};

class LoadLocalBoolVarValueCmd: public LoadLocalVarValueCmd {
 public:
  using LoadLocalVarValueCmd::LoadLocalVarValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadLocalBoolVarValue(*this);
  }
};
}
}
#endif
