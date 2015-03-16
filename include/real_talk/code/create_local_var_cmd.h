
#ifndef _REAL_TALK_CODE_CREATE_LOCAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_LOCAL_VAR_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateLocalVarCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class CreateLocalIntVarCmd: public CreateLocalVarCmd {};

class CreateLocalArrayVarCmd: public CreateLocalVarCmd {};

class CreateLocalLongVarCmd: public CreateLocalVarCmd {};

class CreateLocalDoubleVarCmd: public CreateLocalVarCmd {};

class CreateLocalCharVarCmd: public CreateLocalVarCmd {};

class CreateLocalStringVarCmd: public CreateLocalVarCmd {};

class CreateLocalBoolVarCmd: public CreateLocalVarCmd {};
}
}
#endif
