
#ifndef _REAL_TALK_CODE_CREATE_LOCAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_LOCAL_VAR_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateLocalVarCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class CreateLocalIntVarCmd: public CreateLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateLocalIntVar(*this);
  }
};

class CreateLocalArrayVarCmd: public CreateLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateLocalArrayVar(*this);
  }
};

class CreateLocalLongVarCmd: public CreateLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateLocalLongVar(*this);
  }
};

class CreateLocalDoubleVarCmd: public CreateLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateLocalDoubleVar(*this);
  }
};

class CreateLocalCharVarCmd: public CreateLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateLocalCharVar(*this);
  }
};

class CreateLocalStringVarCmd: public CreateLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateLocalStringVar(*this);
  }
};

class CreateLocalBoolVarCmd: public CreateLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateLocalBoolVar(*this);
  }
};
}
}
#endif
