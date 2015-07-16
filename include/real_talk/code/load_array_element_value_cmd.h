
#ifndef _REAL_TALK_CODE_LOAD_ELEMENT_VALUE_CMD_H_
#define _REAL_TALK_CODE_LOAD_ELEMENT_VALUE_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadArrayElementValueCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class LoadArrayOfIntsElementValueCmd: public LoadArrayElementValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfIntsElementValue(*this);
  }
};

class LoadArrayOfLongsElementValueCmd: public LoadArrayElementValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfLongsElementValue(*this);
  }
};

class LoadArrayOfDoublesElementValueCmd: public LoadArrayElementValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfDoublesElementValue(*this);
  }
};

class LoadArrayOfCharsElementValueCmd: public LoadArrayElementValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfCharsElementValue(*this);
  }
};

class LoadArrayOfBoolsElementValueCmd: public LoadArrayElementValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfBoolsElementValue(*this);
  }
};

class LoadArrayOfStringsElementValueCmd: public LoadArrayElementValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfStringsElementValue(*this);
  }
};

class LoadArrayOfArraysElementValueCmd: public LoadArrayElementValueCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfArraysElementValue(*this);
  }
};
}
}
#endif
