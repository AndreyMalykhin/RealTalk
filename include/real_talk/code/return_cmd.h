
#ifndef _REAL_TALK_CODE_RETURN_CMD_H_
#define _REAL_TALK_CODE_RETURN_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class ReturnCmd: public Cmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitReturn(*this);
  }

 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class ReturnValueCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class ReturnIntValueCmd: public ReturnValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitReturnIntValue(*this);
  }
};

class ReturnLongValueCmd: public ReturnValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitReturnLongValue(*this);
  }
};

class ReturnDoubleValueCmd: public ReturnValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitReturnDoubleValue(*this);
  }
};

class ReturnBoolValueCmd: public ReturnValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitReturnBoolValue(*this);
  }
};

class ReturnCharValueCmd: public ReturnValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitReturnCharValue(*this);
  }
};

class ReturnStringValueCmd: public ReturnValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitReturnStringValue(*this);
  }
};

class ReturnArrayValueCmd: public ReturnValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitReturnArrayValue(*this);
  }
};
}
}
#endif
