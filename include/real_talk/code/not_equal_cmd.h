
#ifndef _REAL_TALK_CODE_NOT_EQUAL_CMD_H_
#define _REAL_TALK_CODE_NOT_EQUAL_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class NotEqualCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class NotEqualIntCmd: public NotEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualInt(*this);
  }
};

class NotEqualLongCmd: public NotEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualLong(*this);
  }
};

class NotEqualDoubleCmd: public NotEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualDouble(*this);
  }
};

class NotEqualBoolCmd: public NotEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualBool(*this);
  }
};

class NotEqualCharCmd: public NotEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualChar(*this);
  }
};

class NotEqualStringCmd: public NotEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualString(*this);
  }
};

class NotEqualArrayCmd: public NotEqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualArray(*this);
  }
};
}
}
#endif
