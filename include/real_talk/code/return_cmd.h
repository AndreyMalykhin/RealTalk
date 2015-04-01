
#ifndef _REAL_TALK_CODE_RETURN_CMD_H_
#define _REAL_TALK_CODE_RETURN_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class ReturnCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};
}
}
#endif
