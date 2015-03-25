
#ifndef _REAL_TALK_CODE_CREATE_GLOBAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_GLOBAL_VAR_CMD_H_

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
  inline explicit CreateGlobalIntVarCmd(uint32_t var_index) noexcept
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalArrayVarCmd: public CreateGlobalVarCmd {
 public:
  inline explicit CreateGlobalArrayVarCmd(uint32_t var_index) noexcept
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalLongVarCmd: public CreateGlobalVarCmd {
 public:
  inline explicit CreateGlobalLongVarCmd(uint32_t var_index) noexcept
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalDoubleVarCmd: public CreateGlobalVarCmd {
 public:
  inline explicit CreateGlobalDoubleVarCmd(uint32_t var_index) noexcept
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalCharVarCmd: public CreateGlobalVarCmd {
 public:
  inline explicit CreateGlobalCharVarCmd(uint32_t var_index) noexcept
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalStringVarCmd: public CreateGlobalVarCmd {
 public:
  inline explicit CreateGlobalStringVarCmd(uint32_t var_index) noexcept
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalBoolVarCmd: public CreateGlobalVarCmd {
 public:
  inline explicit CreateGlobalBoolVarCmd(uint32_t var_index) noexcept
      : CreateGlobalVarCmd(var_index) {}
};
}
}
#endif
