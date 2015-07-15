
#ifndef _REAL_TALK_CODE_AND_CMD_H_
#define _REAL_TALK_CODE_AND_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class AndCmd: public Cmd {
 private:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitAnd(*this);
  }

  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};
}
}
#endif
