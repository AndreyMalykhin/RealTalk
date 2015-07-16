
#ifndef _REAL_TALK_CODE_UNLOAD_CMD_H_
#define _REAL_TALK_CODE_UNLOAD_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class UnloadCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class UnloadIntCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadInt(*this);
  }
};

class UnloadArrayCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadArray(*this);
  }
};

class UnloadLongCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadLong(*this);
  }
};

class UnloadDoubleCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadDouble(*this);
  }
};

class UnloadCharCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadChar(*this);
  }
};

class UnloadStringCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadString(*this);
  }
};

class UnloadBoolCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadBool(*this);
  }
};
}
}
#endif
