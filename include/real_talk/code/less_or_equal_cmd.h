
#ifndef _REAL_TALK_CODE_LESS_OR_EQUAL_CMD_H_
#define _REAL_TALK_CODE_LESS_OR_EQUAL_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LessOrEqualCharCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LessOrEqualIntCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LessOrEqualLongCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LessOrEqualDoubleCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};
}
}
#endif
