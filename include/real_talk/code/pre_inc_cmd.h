
#ifndef _REAL_TALK_CODE_PRE_INC_CMD_H_
#define _REAL_TALK_CODE_PRE_INC_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class PreIncCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class PreIncCharCmd: public PreIncCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitPreIncChar(*this);
  }
};

class PreIncIntCmd: public PreIncCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitPreIncInt(*this);
  }
};

class PreIncLongCmd: public PreIncCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitPreIncLong(*this);
  }
};

class PreIncDoubleCmd: public PreIncCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitPreIncDouble(*this);
  }
};
}
}
#endif
