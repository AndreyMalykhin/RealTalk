
#ifndef _REAL_TALK_CODE_PRE_DEC_CMD_H_
#define _REAL_TALK_CODE_PRE_DEC_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class PreDecCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class PreDecCharCmd: public PreDecCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitPreDecChar(*this);
  }
};

class PreDecIntCmd: public PreDecCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitPreDecInt(*this);
  }
};

class PreDecLongCmd: public PreDecCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitPreDecLong(*this);
  }
};

class PreDecDoubleCmd: public PreDecCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitPreDecDouble(*this);
  }
};
}
}
#endif
