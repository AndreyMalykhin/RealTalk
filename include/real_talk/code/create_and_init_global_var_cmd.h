
#ifndef _REAL_TALK_CODE_CREATE_AND_INIT_GLOBAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_AND_INIT_GLOBAL_VAR_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateAndInitGlobalVarCmd: public Cmd {
 public:
  explicit CreateAndInitGlobalVarCmd(uint32_t var_index)
      : var_index_(var_index) {}
  void SetVarIndex(uint32_t var_index) {var_index_ = var_index;}
  uint32_t GetVarIndex() const {return var_index_;}

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
  explicit CreateAndInitGlobalIntVarCmd(uint32_t var_index)
      : CreateAndInitGlobalVarCmd(var_index) {}
};

class CreateAndInitGlobalArrayVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  explicit CreateAndInitGlobalArrayVarCmd(uint32_t var_index)
      : CreateAndInitGlobalVarCmd(var_index) {}
};

class CreateAndInitGlobalLongVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  explicit CreateAndInitGlobalLongVarCmd(uint32_t var_index)
      : CreateAndInitGlobalVarCmd(var_index) {}
};

class CreateAndInitGlobalDoubleVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  explicit CreateAndInitGlobalDoubleVarCmd(uint32_t var_index)
      : CreateAndInitGlobalVarCmd(var_index) {}
};

class CreateAndInitGlobalCharVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  explicit CreateAndInitGlobalCharVarCmd(uint32_t var_index)
      : CreateAndInitGlobalVarCmd(var_index) {}
};

class CreateAndInitGlobalStringVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  explicit CreateAndInitGlobalStringVarCmd(uint32_t var_index)
      : CreateAndInitGlobalVarCmd(var_index) {}
};

class CreateAndInitGlobalBoolVarCmd: public CreateAndInitGlobalVarCmd {
 public:
  explicit CreateAndInitGlobalBoolVarCmd(uint32_t var_index)
      : CreateAndInitGlobalVarCmd(var_index) {}
};
}
}
#endif
