
#ifndef _REAL_TALK_CODE_CREATE_AND_INIT_LOCAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_AND_INIT_LOCAL_VAR_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateAndInitLocalVarCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class CreateAndInitLocalIntVarCmd: public CreateAndInitLocalVarCmd {};
class CreateAndInitLocalArrayVarCmd: public CreateAndInitLocalVarCmd {};
class CreateAndInitLocalLongVarCmd: public CreateAndInitLocalVarCmd {};
class CreateAndInitLocalDoubleVarCmd: public CreateAndInitLocalVarCmd {};
class CreateAndInitLocalCharVarCmd: public CreateAndInitLocalVarCmd {};
class CreateAndInitLocalStringVarCmd: public CreateAndInitLocalVarCmd {};
class CreateAndInitLocalBoolVarCmd: public CreateAndInitLocalVarCmd {};
}
}
#endif
