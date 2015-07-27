
#ifndef _REAL_TALK_CODE_LOAD_ARRAY_ELEMENT_VALUE_CMD_H_
#define _REAL_TALK_CODE_LOAD_ARRAY_ELEMENT_VALUE_CMD_H_

#include <cstdint>
#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/array_cmd_trait.h"

namespace real_talk {
namespace code {

class LoadArrayElementValueCmd: public Cmd, public ArrayCmdTrait {
 public:
  using ArrayCmdTrait::ArrayCmdTrait;

 private:
  virtual void Print(std::ostream &stream) const override {
    ArrayCmdTrait::Print(stream);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const auto &rhs = static_cast<const LoadArrayElementValueCmd&>(cmd);
    return ArrayCmdTrait::IsEqual(rhs);
  }
};

class LoadIntArrayElementValueCmd: public LoadArrayElementValueCmd {
 public:
  using LoadArrayElementValueCmd::LoadArrayElementValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadIntArrayElementValue(*this);
  }
};

class LoadLongArrayElementValueCmd: public LoadArrayElementValueCmd {
 public:
  using LoadArrayElementValueCmd::LoadArrayElementValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadLongArrayElementValue(*this);
  }
};

class LoadDoubleArrayElementValueCmd: public LoadArrayElementValueCmd {
 public:
  using LoadArrayElementValueCmd::LoadArrayElementValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadDoubleArrayElementValue(*this);
  }
};

class LoadCharArrayElementValueCmd: public LoadArrayElementValueCmd {
 public:
  using LoadArrayElementValueCmd::LoadArrayElementValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadCharArrayElementValue(*this);
  }
};

class LoadBoolArrayElementValueCmd: public LoadArrayElementValueCmd {
 public:
  using LoadArrayElementValueCmd::LoadArrayElementValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadBoolArrayElementValue(*this);
  }
};

class LoadStringArrayElementValueCmd: public LoadArrayElementValueCmd {
 public:
  using LoadArrayElementValueCmd::LoadArrayElementValueCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadStringArrayElementValue(*this);
  }
};
}
}
#endif
