
#ifndef _REAL_TALK_CODE_LESS_OR_EQUAL_CMD_H_
#define _REAL_TALK_CODE_LESS_OR_EQUAL_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LessOrEqualCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LessOrEqualCharCmd: public LessOrEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLessOrEqualChar(*this);
  }
};

class LessOrEqualIntCmd: public LessOrEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLessOrEqualInt(*this);
  }
};

class LessOrEqualLongCmd: public LessOrEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLessOrEqualLong(*this);
  }
};

class LessOrEqualDoubleCmd: public LessOrEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLessOrEqualDouble(*this);
  }
};
}
}
#endif
