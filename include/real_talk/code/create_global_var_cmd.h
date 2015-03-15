
#ifndef _REAL_TALK_CODE_CREATE_GLOBAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_GLOBAL_VAR_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateGlobalVarCmd: public Cmd {
 public:
  explicit CreateGlobalVarCmd(uint32_t var_index): var_index_(var_index) {}

  void SetVarIndex(uint32_t var_index) {
    var_index_ = var_index;
  }

  uint32_t GetVarIndex() const {
    return var_index_;
  }

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
  explicit CreateGlobalIntVarCmd(uint32_t var_index)
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalArrayVarCmd: public CreateGlobalVarCmd {
 public:
  explicit CreateGlobalArrayVarCmd(uint32_t var_index)
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalLongVarCmd: public CreateGlobalVarCmd {
 public:
  explicit CreateGlobalLongVarCmd(uint32_t var_index)
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalDoubleVarCmd: public CreateGlobalVarCmd {
 public:
  explicit CreateGlobalDoubleVarCmd(uint32_t var_index)
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalCharVarCmd: public CreateGlobalVarCmd {
 public:
  explicit CreateGlobalCharVarCmd(uint32_t var_index)
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalStringVarCmd: public CreateGlobalVarCmd {
 public:
  explicit CreateGlobalStringVarCmd(uint32_t var_index)
      : CreateGlobalVarCmd(var_index) {}
};

class CreateGlobalBoolVarCmd: public CreateGlobalVarCmd {
 public:
  explicit CreateGlobalBoolVarCmd(uint32_t var_index)
      : CreateGlobalVarCmd(var_index) {}
};
}
}
#endif
