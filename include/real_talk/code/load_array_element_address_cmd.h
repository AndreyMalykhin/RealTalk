
#ifndef _REAL_TALK_CODE_LOAD_ELEMENT_ADDRESS_CMD_H_
#define _REAL_TALK_CODE_LOAD_ELEMENT_ADDRESS_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadArrayElementAddressCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfIntsElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfIntsElementAddress(*this);
  }
};

class LoadArrayOfLongsElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfLongsElementAddress(*this);
  }
};

class LoadArrayOfDoublesElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfDoublesElementAddress(*this);
  }
};

class LoadArrayOfCharsElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfCharsElementAddress(*this);
  }
};

class LoadArrayOfBoolsElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfBoolsElementAddress(*this);
  }
};

class LoadArrayOfStringsElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfStringsElementAddress(*this);
  }
};

class LoadArrayOfArraysElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfArraysElementAddress(*this);
  }
};
}
}
#endif
