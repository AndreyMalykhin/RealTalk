
#ifndef _REAL_TALK_CODE_CREATE_GLOBAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_GLOBAL_VAR_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateGlobalVarCmd: public Cmd {
 public:
  inline explicit CreateGlobalVarCmd(uint32_t var_index) noexcept
      : var_index_(var_index) {}

  inline void SetVarIndex(uint32_t var_index) noexcept {var_index_ = var_index;}
  inline uint32_t GetVarIndex() const noexcept {return var_index_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "var_index=" << var_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const CreateGlobalVarCmd &rhs =
        static_cast<const CreateGlobalVarCmd&>(cmd);
    return var_index_ == rhs.var_index_;
  }

  uint32_t var_index_;
};

class CreateGlobalIntVarCmd: public CreateGlobalVarCmd {
 public:
  using CreateGlobalVarCmd::CreateGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateGlobalIntVar(*this);
  }
};

class CreateGlobalArrayVarCmd: public CreateGlobalVarCmd {
 public:
  using CreateGlobalVarCmd::CreateGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateGlobalArrayVar(*this);
  }
};

class CreateGlobalLongVarCmd: public CreateGlobalVarCmd {
 public:
  using CreateGlobalVarCmd::CreateGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateGlobalLongVar(*this);
  }
};

class CreateGlobalDoubleVarCmd: public CreateGlobalVarCmd {
 public:
  using CreateGlobalVarCmd::CreateGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateGlobalDoubleVar(*this);
  }
};

class CreateGlobalCharVarCmd: public CreateGlobalVarCmd {
 public:
  using CreateGlobalVarCmd::CreateGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateGlobalCharVar(*this);
  }
};

class CreateGlobalStringVarCmd: public CreateGlobalVarCmd {
 public:
  using CreateGlobalVarCmd::CreateGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateGlobalStringVar(*this);
  }
};

class CreateGlobalBoolVarCmd: public CreateGlobalVarCmd {
 public:
  using CreateGlobalVarCmd::CreateGlobalVarCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCreateGlobalBoolVar(*this);
  }
};
}
}
#endif
