
#ifndef _REAL_TALK_CODE_CREATE_AND_INIT_ARRAY_CMD_H_
#define _REAL_TALK_CODE_CREATE_AND_INIT_ARRAY_CMD_H_

#include <cassert>
#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/array_cmd_trait.h"

namespace real_talk {
namespace code {

class CreateAndInitArrayCmd: public Cmd, public ArrayCmdTrait {
 public:
  inline explicit CreateAndInitArrayCmd(
      uint8_t dimensions_count, int32_t values_count) noexcept
      : ArrayCmdTrait(dimensions_count), values_count_(values_count) {}

  inline int32_t GetValuesCount() const noexcept {
    return values_count_;
  }

  inline void SetValuesCount(int32_t values_count) noexcept {
    values_count_ = values_count;
  }

 private:
  virtual void Print(std::ostream &stream) const override {
    ArrayCmdTrait::Print(stream);
    stream << "; values_count=" << values_count_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const CreateAndInitArrayCmd &rhs =
        static_cast<const CreateAndInitArrayCmd&>(cmd);
    return ArrayCmdTrait::IsEqual(rhs) && values_count_ == rhs.values_count_;
  }

  int32_t values_count_;
};

class CreateAndInitIntArrayCmd: public CreateAndInitArrayCmd {
 public:
  using CreateAndInitArrayCmd::CreateAndInitArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitIntArray(*this);
  }
};

class CreateAndInitLongArrayCmd: public CreateAndInitArrayCmd {
 public:
  using CreateAndInitArrayCmd::CreateAndInitArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitLongArray(*this);
  }
};

class CreateAndInitDoubleArrayCmd: public CreateAndInitArrayCmd {
 public:
  using CreateAndInitArrayCmd::CreateAndInitArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitDoubleArray(*this);
  }
};

class CreateAndInitBoolArrayCmd: public CreateAndInitArrayCmd {
 public:
  using CreateAndInitArrayCmd::CreateAndInitArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitBoolArray(*this);
  }
};

class CreateAndInitCharArrayCmd: public CreateAndInitArrayCmd {
 public:
  using CreateAndInitArrayCmd::CreateAndInitArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitCharArray(*this);
  }
};

class CreateAndInitStringArrayCmd: public CreateAndInitArrayCmd {
 public:
  using CreateAndInitArrayCmd::CreateAndInitArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitStringArray(*this);
  }
};
}
}
#endif
