
#ifndef _REAL_TALK_CODE_LOAD_LOCAL_VAR_VALUE_CMD_H_
#define _REAL_TALK_CODE_LOAD_LOCAL_VAR_VALUE_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadLocalVarValueCmd: public Cmd {
 public:
  inline explicit LoadLocalVarValueCmd(uint32_t var_index) noexcept
      : var_index_(var_index) {}

  inline void SetVarIndex(uint32_t var_index) noexcept {var_index_ = var_index;}
  inline uint32_t GetVarIndex() const noexcept {return var_index_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "var_index=" << var_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadLocalVarValueCmd &rhs =
        static_cast<const LoadLocalVarValueCmd&>(cmd);
    return var_index_ == rhs.var_index_;
  }

  uint32_t var_index_;
};

class LoadLocalIntVarValueCmd: public LoadLocalVarValueCmd {
 public:
  inline explicit LoadLocalIntVarValueCmd(uint32_t var_index) noexcept
      : LoadLocalVarValueCmd(var_index) {}
};

class LoadLocalArrayVarValueCmd: public LoadLocalVarValueCmd {
 public:
  inline explicit LoadLocalArrayVarValueCmd(uint32_t var_index) noexcept
      : LoadLocalVarValueCmd(var_index) {}
};

class LoadLocalLongVarValueCmd: public LoadLocalVarValueCmd {
 public:
  inline explicit LoadLocalLongVarValueCmd(uint32_t var_index) noexcept
      : LoadLocalVarValueCmd(var_index) {}
};

class LoadLocalDoubleVarValueCmd: public LoadLocalVarValueCmd {
 public:
  inline explicit LoadLocalDoubleVarValueCmd(uint32_t var_index) noexcept
      : LoadLocalVarValueCmd(var_index) {}
};

class LoadLocalCharVarValueCmd: public LoadLocalVarValueCmd {
 public:
  inline explicit LoadLocalCharVarValueCmd(uint32_t var_index) noexcept
      : LoadLocalVarValueCmd(var_index) {}
};

class LoadLocalStringVarValueCmd: public LoadLocalVarValueCmd {
 public:
  inline explicit LoadLocalStringVarValueCmd(uint32_t var_index) noexcept
      : LoadLocalVarValueCmd(var_index) {}
};

class LoadLocalBoolVarValueCmd: public LoadLocalVarValueCmd {
 public:
  inline explicit LoadLocalBoolVarValueCmd(uint32_t var_index) noexcept
      : LoadLocalVarValueCmd(var_index) {}
};
}
}
#endif
