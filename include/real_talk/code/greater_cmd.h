
#ifndef _REAL_TALK_CODE_GREATER_CMD_H_
#define _REAL_TALK_CODE_GREATER_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class GreaterCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class GreaterCharCmd: public GreaterCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitGreaterChar(*this);
  }
};

class GreaterIntCmd: public GreaterCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitGreaterInt(*this);
  }
};

class GreaterLongCmd: public GreaterCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitGreaterLong(*this);
  }
};

class GreaterDoubleCmd: public GreaterCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitGreaterDouble(*this);
  }
};
}
}
#endif
