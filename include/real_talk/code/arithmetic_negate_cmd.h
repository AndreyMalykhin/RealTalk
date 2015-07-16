
#ifndef _REAL_TALK_CODE_ARITHMETIC_NEGATE_CMD_H_
#define _REAL_TALK_CODE_ARITHMETIC_NEGATE_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class ArithmeticNegateCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class ArithmeticNegateIntCmd: public ArithmeticNegateCmd {
 private:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitArithmeticNegateInt(*this);
  }
};

class ArithmeticNegateLongCmd: public ArithmeticNegateCmd {
 private:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitArithmeticNegateLong(*this);
  }
};

class ArithmeticNegateDoubleCmd: public ArithmeticNegateCmd {
 private:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitArithmeticNegateDouble(*this);
  }
};
}
}
#endif
