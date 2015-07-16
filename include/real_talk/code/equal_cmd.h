
#ifndef _REAL_TALK_CODE_EQUAL_CMD_H_
#define _REAL_TALK_CODE_EQUAL_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class EqualCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class EqualIntCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualInt(*this);
  }
};

class EqualLongCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualLong(*this);
  }
};

class EqualDoubleCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualDouble(*this);
  }
};

class EqualBoolCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualBool(*this);
  }
};

class EqualCharCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualChar(*this);
  }
};

class EqualStringCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualString(*this);
  }
};

class EqualArrayCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualArray(*this);
  }
};
}
}
#endif
