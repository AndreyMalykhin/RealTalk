
#ifndef _REAL_TALK_CODE_LOGICAL_NEGATE_CMD_H_
#define _REAL_TALK_CODE_LOGICAL_NEGATE_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LogicalNegateBoolCmd: public Cmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLogicalNegateBool(*this);
  }

 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};
}
}
#endif
