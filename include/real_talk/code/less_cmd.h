
#ifndef _REAL_TALK_CODE_LESS_CMD_H_
#define _REAL_TALK_CODE_LESS_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LessCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LessCharCmd: public LessCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLessChar(*this);
  }
};

class LessIntCmd: public LessCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLessInt(*this);
  }
};

class LessLongCmd: public LessCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLessLong(*this);
  }
};

class LessDoubleCmd: public LessCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLessDouble(*this);
  }
};
}
}
#endif
