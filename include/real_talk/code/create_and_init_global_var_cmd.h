
#ifndef _REAL_TALK_CODE_CREATE_AND_INIT_GLOBAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_AND_INIT_GLOBAL_VAR_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateAndInitGlobalVarCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class CreateAndInitGlobalIntVarCmd: public CreateAndInitGlobalVarCmd {};
class CreateAndInitGlobalArrayVarCmd: public CreateAndInitGlobalVarCmd {};
class CreateAndInitGlobalLongVarCmd: public CreateAndInitGlobalVarCmd {};
class CreateAndInitGlobalDoubleVarCmd: public CreateAndInitGlobalVarCmd {};
class CreateAndInitGlobalCharVarCmd: public CreateAndInitGlobalVarCmd {};
class CreateAndInitGlobalStringVarCmd: public CreateAndInitGlobalVarCmd {};
class CreateAndInitGlobalBoolVarCmd: public CreateAndInitGlobalVarCmd {};
}
}
#endif
