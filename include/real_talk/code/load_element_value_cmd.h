
#ifndef _REAL_TALK_CODE_LOAD_ELEMENT_VALUE_CMD_H_
#define _REAL_TALK_CODE_LOAD_ELEMENT_VALUE_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadArrayOfIntsElementValueCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfLongsElementValueCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfDoublesElementValueCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfCharsElementValueCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfBoolsElementValueCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfStringsElementValueCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfArraysElementValueCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};
}
}
#endif
