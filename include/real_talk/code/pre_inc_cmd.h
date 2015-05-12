
#ifndef _REAL_TALK_CODE_PRE_INC_CMD_H_
#define _REAL_TALK_CODE_PRE_INC_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class PreIncCharCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class PreIncIntCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class PreIncLongCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class PreIncDoubleCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};
}
}
#endif
