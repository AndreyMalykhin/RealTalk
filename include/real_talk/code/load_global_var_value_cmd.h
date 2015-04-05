
#ifndef _REAL_TALK_CODE_LOAD_GLOBAL_VAR_VALUE_CMD_H_
#define _REAL_TALK_CODE_LOAD_GLOBAL_VAR_VALUE_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadGlobalVarValueCmd: public Cmd {
 public:
  inline explicit LoadGlobalVarValueCmd(uint32_t var_index) noexcept
      : var_index_(var_index) {}

  inline void SetVarIndex(uint32_t var_index) noexcept {var_index_ = var_index;}
  inline uint32_t GetVarIndex() const noexcept {return var_index_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "var_index=" << var_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadGlobalVarValueCmd &rhs =
        static_cast<const LoadGlobalVarValueCmd&>(cmd);
    return var_index_ == rhs.var_index_;
  }

  uint32_t var_index_;
};

class LoadGlobalIntVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  inline explicit LoadGlobalIntVarValueCmd(uint32_t var_index) noexcept
      : LoadGlobalVarValueCmd(var_index) {}
};

class LoadGlobalArrayVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  inline explicit LoadGlobalArrayVarValueCmd(uint32_t var_index) noexcept
      : LoadGlobalVarValueCmd(var_index) {}
};

class LoadGlobalLongVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  inline explicit LoadGlobalLongVarValueCmd(uint32_t var_index) noexcept
      : LoadGlobalVarValueCmd(var_index) {}
};

class LoadGlobalDoubleVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  inline explicit LoadGlobalDoubleVarValueCmd(uint32_t var_index) noexcept
      : LoadGlobalVarValueCmd(var_index) {}
};

class LoadGlobalCharVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  inline explicit LoadGlobalCharVarValueCmd(uint32_t var_index) noexcept
      : LoadGlobalVarValueCmd(var_index) {}
};

class LoadGlobalStringVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  inline explicit LoadGlobalStringVarValueCmd(uint32_t var_index) noexcept
      : LoadGlobalVarValueCmd(var_index) {}
};

class LoadGlobalBoolVarValueCmd: public LoadGlobalVarValueCmd {
 public:
  inline explicit LoadGlobalBoolVarValueCmd(uint32_t var_index) noexcept
      : LoadGlobalVarValueCmd(var_index) {}
};
}
}
#endif
