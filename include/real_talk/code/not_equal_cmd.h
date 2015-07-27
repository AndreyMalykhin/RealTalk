
#ifndef _REAL_TALK_CODE_NOT_EQUAL_CMD_H_
#define _REAL_TALK_CODE_NOT_EQUAL_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/array_cmd_trait.h"

namespace real_talk {
namespace code {

class NotEqualCmd: public Cmd {
 protected:
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

class NotEqualArrayCmd: public NotEqualCmd, public ArrayCmdTrait {
 public:
  using ArrayCmdTrait::ArrayCmdTrait;

 private:
  virtual void Print(std::ostream &stream) const override {
    NotEqualCmd::Print(stream);
    ArrayCmdTrait::Print(stream);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const auto &rhs = static_cast<const NotEqualArrayCmd&>(cmd);
    return NotEqualCmd::IsEqual(cmd) && ArrayCmdTrait::IsEqual(rhs);
  }
};

class NotEqualIntArrayCmd: public NotEqualArrayCmd {
 public:
  using NotEqualArrayCmd::NotEqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualIntArray(*this);
  }
};

class NotEqualLongArrayCmd: public NotEqualArrayCmd {
 public:
  using NotEqualArrayCmd::NotEqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualLongArray(*this);
  }
};

class NotEqualDoubleArrayCmd: public NotEqualArrayCmd {
 public:
  using NotEqualArrayCmd::NotEqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualDoubleArray(*this);
  }
};

class NotEqualCharArrayCmd: public NotEqualArrayCmd {
 public:
  using NotEqualArrayCmd::NotEqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualCharArray(*this);
  }
};

class NotEqualStringArrayCmd: public NotEqualArrayCmd {
 public:
  using NotEqualArrayCmd::NotEqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualStringArray(*this);
  }
};

class NotEqualBoolArrayCmd: public NotEqualArrayCmd {
 public:
  using NotEqualArrayCmd::NotEqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitNotEqualBoolArray(*this);
  }
};
}
}
#endif
