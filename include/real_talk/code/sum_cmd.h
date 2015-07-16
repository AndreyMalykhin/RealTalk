
#ifndef _REAL_TALK_CODE_SUM_CMD_H_
#define _REAL_TALK_CODE_SUM_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class SumCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class SumCharCmd: public SumCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitSumChar(*this);
  }
};

class SumIntCmd: public SumCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitSumInt(*this);
  }
};

class SumLongCmd: public SumCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitSumLong(*this);
  }
};

class SumDoubleCmd: public SumCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitSumDouble(*this);
  }
};

class SumStringCmd: public SumCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitSumString(*this);
  }
};
}
}
#endif
