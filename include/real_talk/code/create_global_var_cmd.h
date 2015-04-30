
#ifndef _REAL_TALK_CODE_CREATE_GLOBAL_VAR_CMD_H_
#define _REAL_TALK_CODE_CREATE_GLOBAL_VAR_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateGlobalVarCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class CreateGlobalIntVarCmd: public CreateGlobalVarCmd {};
class CreateGlobalArrayVarCmd: public CreateGlobalVarCmd {};
class CreateGlobalLongVarCmd: public CreateGlobalVarCmd {};
class CreateGlobalDoubleVarCmd: public CreateGlobalVarCmd {};
class CreateGlobalCharVarCmd: public CreateGlobalVarCmd {};
class CreateGlobalStringVarCmd: public CreateGlobalVarCmd {};
class CreateGlobalBoolVarCmd: public CreateGlobalVarCmd {};
}
}
#endif
