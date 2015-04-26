
#ifndef _REAL_TALK_CODE_LOAD_ELEMENT_ADDRESS_CMD_H_
#define _REAL_TALK_CODE_LOAD_ELEMENT_ADDRESS_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadArrayOfIntsElementAddressCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfLongsElementAddressCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfDoublesElementAddressCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfCharsElementAddressCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfBoolsElementAddressCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfStringsElementAddressCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfArraysElementAddressCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};
}
}
#endif
