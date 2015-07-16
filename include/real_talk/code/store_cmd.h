
#ifndef _REAL_TALK_CODE_STORE_CMD_H_
#define _REAL_TALK_CODE_STORE_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class StoreCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class StoreIntCmd: public StoreCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreInt(*this);
  }
};

class StoreLongCmd: public StoreCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreLong(*this);
  }
};

class StoreDoubleCmd: public StoreCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreDouble(*this);
  }
};

class StoreBoolCmd: public StoreCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreBool(*this);
  }
};

class StoreCharCmd: public StoreCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreChar(*this);
  }
};

class StoreStringCmd: public StoreCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreString(*this);
  }
};

class StoreArrayCmd: public StoreCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreArray(*this);
  }
};
}
}
#endif
