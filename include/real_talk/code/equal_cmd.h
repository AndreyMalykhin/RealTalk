
#ifndef _REAL_TALK_CODE_EQUAL_CMD_H_
#define _REAL_TALK_CODE_EQUAL_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class EqualCmd: public Cmd {
 protected:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class EqualIntCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualInt(*this);
  }
};

class EqualLongCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualLong(*this);
  }
};

class EqualDoubleCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualDouble(*this);
  }
};

class EqualBoolCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualBool(*this);
  }
};

class EqualCharCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualChar(*this);
  }
};

class EqualStringCmd: public EqualCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualString(*this);
  }
};

class EqualArrayCmd: public EqualCmd {
 public:
  inline explicit EqualArrayCmd(uint8_t dimensions_count) noexcept
      : dimensions_count_(dimensions_count) {
        assert(dimensions_count != UINT8_C(0));
      }

  inline uint8_t GetDimensionsCount() const noexcept {return dimensions_count_;}

  inline void SetDimensionsCount(uint8_t count) noexcept {
    dimensions_count_ = count;
  }

 private:
  virtual void Print(std::ostream &stream) const override {
    EqualCmd::Print(stream);
    stream << "dimensions_count=" << static_cast<int>(dimensions_count_);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const auto &rhs = static_cast<const EqualArrayCmd&>(cmd);
    return EqualCmd::IsEqual(cmd)
        && dimensions_count_ == rhs.dimensions_count_;
  }

  uint8_t dimensions_count_;
};

class EqualIntArrayCmd: public EqualArrayCmd {
 public:
  using EqualArrayCmd::EqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualIntArray(*this);
  }
};

class EqualLongArrayCmd: public EqualArrayCmd {
 public:
  using EqualArrayCmd::EqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualLongArray(*this);
  }
};

class EqualDoubleArrayCmd: public EqualArrayCmd {
 public:
  using EqualArrayCmd::EqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualDoubleArray(*this);
  }
};

class EqualCharArrayCmd: public EqualArrayCmd {
 public:
  using EqualArrayCmd::EqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualCharArray(*this);
  }
};

class EqualStringArrayCmd: public EqualArrayCmd {
 public:
  using EqualArrayCmd::EqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualStringArray(*this);
  }
};

class EqualBoolArrayCmd: public EqualArrayCmd {
 public:
  using EqualArrayCmd::EqualArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitEqualBoolArray(*this);
  }
};
}
}
#endif
