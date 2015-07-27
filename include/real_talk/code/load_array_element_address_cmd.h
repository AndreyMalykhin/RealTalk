
#ifndef _REAL_TALK_CODE_LOAD_ARRAY_ELEMENT_ADDRESS_CMD_H_
#define _REAL_TALK_CODE_LOAD_ARRAY_ELEMENT_ADDRESS_CMD_H_

#include <cstdint>
#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/array_cmd_trait.h"

namespace real_talk {
namespace code {

class LoadArrayElementAddressCmd: public Cmd, public ArrayCmdTrait {
 public:
  using ArrayCmdTrait::ArrayCmdTrait;

 private:
  virtual void Print(std::ostream &stream) const override {
    ArrayCmdTrait::Print(stream);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const auto &rhs = static_cast<const LoadArrayElementAddressCmd&>(cmd);
    return ArrayCmdTrait::IsEqual(rhs);
  }
};

class LoadIntArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  using LoadArrayElementAddressCmd::LoadArrayElementAddressCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadIntArrayElementAddress(*this);
  }
};

class LoadLongArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  using LoadArrayElementAddressCmd::LoadArrayElementAddressCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadLongArrayElementAddress(*this);
  }
};

class LoadDoubleArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  using LoadArrayElementAddressCmd::LoadArrayElementAddressCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadDoubleArrayElementAddress(*this);
  }
};

class LoadCharArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  using LoadArrayElementAddressCmd::LoadArrayElementAddressCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadCharArrayElementAddress(*this);
  }
};

class LoadBoolArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  using LoadArrayElementAddressCmd::LoadArrayElementAddressCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadBoolArrayElementAddress(*this);
  }
};

class LoadStringArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  using LoadArrayElementAddressCmd::LoadArrayElementAddressCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadStringArrayElementAddress(*this);
  }
};
}
}
#endif
