
#ifndef _REAL_TALK_CODE_SUB_CMD_H_
#define _REAL_TALK_CODE_SUB_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class SubCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class SubCharCmd: public SubCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitSubChar(*this);
  }
};

class SubIntCmd: public SubCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitSubInt(*this);
  }
};

class SubLongCmd: public SubCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitSubLong(*this);
  }
};

class SubDoubleCmd: public SubCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitSubDouble(*this);
  }
};
}
}
#endif
