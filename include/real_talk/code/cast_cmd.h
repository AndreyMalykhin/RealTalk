
#ifndef _REAL_TALK_CODE_CAST_CMD_H_
#define _REAL_TALK_CODE_CAST_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CastCmd: public Cmd {
 private:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class CastCharToIntCmd: public CastCmd {
 private:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCastCharToInt(*this);
  }
};

class CastCharToLongCmd: public CastCmd {
 private:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCastCharToLong(*this);
  }
};

class CastCharToDoubleCmd: public CastCmd {
 private:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCastCharToDouble(*this);
  }
};

class CastCharToStringCmd: public CastCmd {
 private:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCastCharToString(*this);
  }
};

class CastIntToLongCmd: public CastCmd {
 private:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCastIntToLong(*this);
  }
};

class CastIntToDoubleCmd: public CastCmd {
 private:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCastIntToDouble(*this);
  }
};

class CastLongToDoubleCmd: public CastCmd {
 private:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitCastLongToDouble(*this);
  }
};
}
}
#endif
