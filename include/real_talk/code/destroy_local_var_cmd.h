
#ifndef _REAL_TALK_CODE_DESTROY_LOCAL_VAR_CMD_H_
#define _REAL_TALK_CODE_DESTROY_LOCAL_VAR_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class DestroyLocalVarCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class DestroyLocalIntVarCmd: public DestroyLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalIntVar(*this);
  }
};

class DestroyLocalArrayVarCmd: public DestroyLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDestroyLocalArrayVar(*this);
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
}
}
#endif
