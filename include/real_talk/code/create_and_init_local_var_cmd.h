
#ifndef _REAL_TALK_CODE_CREATE_AND_INIT_LOCAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_AND_INIT_LOCAL_VAR_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateAndInitLocalVarCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class CreateAndInitLocalIntVarCmd: public CreateAndInitLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalIntVar(*this);
  }
};

class CreateAndInitLocalArrayVarCmd: public CreateAndInitLocalVarCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLocalArrayVar(*this);
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
}
}
#endif
