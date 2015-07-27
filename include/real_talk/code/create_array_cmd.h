
#ifndef _REAL_TALK_CODE_CREATE_ARRAY_CMD_H_
#define _REAL_TALK_CODE_CREATE_ARRAY_CMD_H_

#include <cassert>
#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/array_cmd_trait.h"

namespace real_talk {
namespace code {

class CreateArrayCmd: public Cmd, public ArrayCmdTrait {
 public:
  using ArrayCmdTrait::ArrayCmdTrait;

 private:
  virtual void Print(std::ostream &stream) const override {
    ArrayCmdTrait::Print(stream);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const CreateArrayCmd &rhs = static_cast<const CreateArrayCmd&>(cmd);
    return ArrayCmdTrait::IsEqual(rhs);
  }
};

class CreateIntArrayCmd: public CreateArrayCmd {
 public:
  using CreateArrayCmd::CreateArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateIntArray(*this);
  }
};

class CreateLongArrayCmd: public CreateArrayCmd {
 public:
  using CreateArrayCmd::CreateArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateLongArray(*this);
  }
};

class CreateDoubleArrayCmd: public CreateArrayCmd {
 public:
  using CreateArrayCmd::CreateArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateDoubleArray(*this);
  }
};

class CreateBoolArrayCmd: public CreateArrayCmd {
 public:
  using CreateArrayCmd::CreateArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateBoolArray(*this);
  }
};

class CreateCharArrayCmd: public CreateArrayCmd {
 public:
  using CreateArrayCmd::CreateArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateCharArray(*this);
  }
};

class CreateStringArrayCmd: public CreateArrayCmd {
 public:
  using CreateArrayCmd::CreateArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateStringArray(*this);
  }
};
}
}
#endif
