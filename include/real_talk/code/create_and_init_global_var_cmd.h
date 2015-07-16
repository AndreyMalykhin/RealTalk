
#ifndef _REAL_TALK_CODE_CREATE_AND_INIT_GLOBAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_AND_INIT_GLOBAL_VAR_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateAndInitGlobalVarCmd: public Cmd {
 public:
  inline explicit CreateAndInitGlobalVarCmd(uint32_t var_index) noexcept
      : var_index_(var_index) {}

  inline void SetVarIndex(uint32_t var_index) noexcept {var_index_ = var_index;}
  inline uint32_t GetVarIndex() const noexcept {return var_index_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "var_index=" << var_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const CreateAndInitGlobalVarCmd &rhs =
        static_cast<const CreateAndInitGlobalVarCmd&>(cmd);
    return var_index_ == rhs.var_index_;
  }

  uint32_t var_index_;
};

class CreateAndInitGlobalIntVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalIntVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalIntVar(*this);
  }
};

class CreateAndInitGlobalArrayVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalArrayVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalArrayVar(*this);
  }
};

class CreateAndInitGlobalLongVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalLongVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalLongVar(*this);
  }
};

class CreateAndInitGlobalDoubleVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalDoubleVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalDoubleVar(*this);
  }
};

class CreateAndInitGlobalCharVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalCharVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalCharVar(*this);
  }
};

class CreateAndInitGlobalStringVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalStringVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalStringVar(*this);
  }
};

class CreateAndInitGlobalBoolVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  inline explicit CreateAndInitGlobalBoolVarCmd(uint32_t var_index) noexcept
      : CreateAndInitGlobalVarCmd(var_index) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateAndInitGlobalBoolVar(*this);
  }
};
}
}
#endif
