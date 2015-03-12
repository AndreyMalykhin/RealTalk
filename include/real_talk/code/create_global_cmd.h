
#ifndef _REAL_TALK_CODE_CREATE_GLOBAL_CMD_H_
#define _REAL_TALK_CODE_CREATE_GLOBAL_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateGlobalIntCmd: public Cmd {
 public:
  explicit CreateGlobalIntCmd(uint32_t var_index): var_index_(var_index) {}

  void SetVarIndex(uint32_t var_index) {
    var_index_ = var_index;
  }

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "var_index=" << var_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const CreateGlobalIntCmd &rhs = static_cast<const CreateGlobalIntCmd&>(cmd);
    return var_index_ == rhs.var_index_;
  }

  uint32_t var_index_;
};
}
}
#endif
