
#ifndef _REAL_TALK_CODE_CREATE_AND_INIT_GLOBAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_AND_INIT_GLOBAL_VAR_CMD_H_

#include <cassert>
#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"
#include "real_talk/code/array_cmd_trait.h"

namespace real_talk {
namespace code {

class CreateAndInitGlobalVarCmd: public Cmd {
 public:
  inline explicit CreateAndInitGlobalVarCmd(uint32_t var_index) noexcept
      : var_index_(var_index) {}

  inline void SetVarIndex(uint32_t var_index) noexcept {var_index_ = var_index;}
  inline uint32_t GetVarIndex() const noexcept {return var_index_;}

 protected:
  virtual void Print(std::ostream &stream) const override {
    stream << "var_index=" << var_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const CreateAndInitGlobalVarCmd &rhs =
        static_cast<const CreateAndInitGlobalVarCmd&>(cmd);
    return var_index_ == rhs.var_index_;
  }

 private:
  uint32_t var_index_;
};

class CreateAndInitGlobalIntVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  using CreateAndInitGlobalVarCmd::CreateAndInitGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalIntVar(*this);
  }
};

class CreateAndInitGlobalLongVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  using CreateAndInitGlobalVarCmd::CreateAndInitGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalLongVar(*this);
  }
};

class CreateAndInitGlobalDoubleVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  using CreateAndInitGlobalVarCmd::CreateAndInitGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalDoubleVar(*this);
  }
};

class CreateAndInitGlobalCharVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  using CreateAndInitGlobalVarCmd::CreateAndInitGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalCharVar(*this);
  }
};

class CreateAndInitGlobalStringVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  using CreateAndInitGlobalVarCmd::CreateAndInitGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalStringVar(*this);
  }
};

class CreateAndInitGlobalBoolVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  using CreateAndInitGlobalVarCmd::CreateAndInitGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalBoolVar(*this);
  }
};

class CreateAndInitGlobalArrayVarCmd: public CreateAndInitGlobalVarCmd,
                                      public ArrayCmdTrait {
 public:
  inline CreateAndInitGlobalArrayVarCmd(
      uint32_t var_index, uint8_t dimensions_count) noexcept
      : CreateAndInitGlobalVarCmd(var_index),
        ArrayCmdTrait(dimensions_count) {}

 private:
  virtual void Print(std::ostream &stream) const override {
    CreateAndInitGlobalVarCmd::Print(stream);
    stream << "; ";
    ArrayCmdTrait::Print(stream);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const auto &rhs = static_cast<const CreateAndInitGlobalArrayVarCmd&>(cmd);
    return CreateAndInitGlobalVarCmd::IsEqual(cmd)
        && ArrayCmdTrait::IsEqual(rhs);
  }
};

class CreateAndInitGlobalIntArrayVarCmd: public CreateAndInitGlobalArrayVarCmd {
 public:
  using CreateAndInitGlobalArrayVarCmd::CreateAndInitGlobalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalIntArrayVar(*this);
  }
};

class CreateAndInitGlobalLongArrayVarCmd
    : public CreateAndInitGlobalArrayVarCmd {
 public:
  using CreateAndInitGlobalArrayVarCmd::CreateAndInitGlobalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalLongArrayVar(*this);
  }
};

class CreateAndInitGlobalDoubleArrayVarCmd
    : public CreateAndInitGlobalArrayVarCmd {
 public:
  using CreateAndInitGlobalArrayVarCmd::CreateAndInitGlobalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalDoubleArrayVar(*this);
  }
};

class CreateAndInitGlobalCharArrayVarCmd
    : public CreateAndInitGlobalArrayVarCmd {
 public:
  using CreateAndInitGlobalArrayVarCmd::CreateAndInitGlobalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalCharArrayVar(*this);
  }
};

class CreateAndInitGlobalStringArrayVarCmd
    : public CreateAndInitGlobalArrayVarCmd {
 public:
  using CreateAndInitGlobalArrayVarCmd::CreateAndInitGlobalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalStringArrayVar(*this);
  }
};

class CreateAndInitGlobalBoolArrayVarCmd
    : public CreateAndInitGlobalArrayVarCmd {
 public:
  using CreateAndInitGlobalArrayVarCmd::CreateAndInitGlobalArrayVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalBoolArrayVar(*this);
  }
};
}
}
#endif
