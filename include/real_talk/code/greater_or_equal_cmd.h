
#ifndef _REAL_TALK_CODE_GREATER_OR_EQUAL_CMD_H_
#define _REAL_TALK_CODE_GREATER_OR_EQUAL_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class GreaterOrEqualCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class GreaterOrEqualCharCmd: public GreaterOrEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitGreaterOrEqualChar(*this);
  }
};

class GreaterOrEqualIntCmd: public GreaterOrEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitGreaterOrEqualInt(*this);
  }
};

class GreaterOrEqualLongCmd: public GreaterOrEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitGreaterOrEqualLong(*this);
  }
};

class GreaterOrEqualDoubleCmd: public GreaterOrEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitGreaterOrEqualDouble(*this);
  }
};
}
}
#endif
