
#ifndef _REAL_TALK_CODE_DESTROY_LOCAL_VAR_CMD_H_
#define _REAL_TALK_CODE_DESTROY_LOCAL_VAR_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class DestroyLocalVarCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class DestroyLocalIntVarCmd: public DestroyLocalVarCmd {};

class DestroyLocalArrayVarCmd: public DestroyLocalVarCmd {};

class DestroyLocalLongVarCmd: public DestroyLocalVarCmd {};

class DestroyLocalDoubleVarCmd: public DestroyLocalVarCmd {};

class DestroyLocalCharVarCmd: public DestroyLocalVarCmd {};

class DestroyLocalStringVarCmd: public DestroyLocalVarCmd {};

class DestroyLocalBoolVarCmd: public DestroyLocalVarCmd {};
}
}
#endif
