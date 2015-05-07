
#ifndef _REAL_TALK_CODE_GREATER_OR_EQUAL_CMD_H_
#define _REAL_TALK_CODE_GREATER_OR_EQUAL_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class GreaterOrEqualCharCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class GreaterOrEqualIntCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class GreaterOrEqualLongCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class GreaterOrEqualDoubleCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};
}
}
#endif
