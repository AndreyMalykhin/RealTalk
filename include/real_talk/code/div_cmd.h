
#ifndef _REAL_TALK_CODE_DIV_CMD_H_
#define _REAL_TALK_CODE_DIV_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class DivCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class DivCharCmd: public DivCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDivChar(*this);
  }
};

class DivIntCmd: public DivCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDivInt(*this);
  }
};

class DivLongCmd: public DivCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDivLong(*this);
  }
};

class DivDoubleCmd: public DivCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitDivDouble(*this);
  }
};
}
}
#endif
