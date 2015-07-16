
#ifndef _REAL_TALK_CODE_MUL_CMD_H_
#define _REAL_TALK_CODE_MUL_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class MulCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class MulCharCmd: public MulCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitMulChar(*this);
  }
};

class MulIntCmd: public MulCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitMulInt(*this);
  }
};

class MulLongCmd: public MulCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitMulLong(*this);
  }
};

class MulDoubleCmd: public MulCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitMulDouble(*this);
  }
};
}
}
#endif
