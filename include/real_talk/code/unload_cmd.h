
#ifndef _REAL_TALK_CODE_UNLOAD_CMD_H_
#define _REAL_TALK_CODE_UNLOAD_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class UnloadCmd: public Cmd {
 protected:
  virtual void Print(std::ostream&) const override {}
  virtual bool IsEqual(const Cmd&) const override {return true;}
};

class UnloadIntCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadInt(*this);
  }
};

class UnloadLongCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadLong(*this);
  }
};

class UnloadDoubleCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadDouble(*this);
  }
};

class UnloadCharCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadChar(*this);
  }
};

class UnloadStringCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadString(*this);
  }
};

class UnloadBoolCmd: public UnloadCmd {
 public:
  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadBool(*this);
  }
};

class UnloadArrayCmd: public UnloadCmd {
 public:
  inline explicit UnloadArrayCmd(uint8_t dimensions_count) noexcept
      : dimensions_count_(dimensions_count) {
        assert(dimensions_count != UINT8_C(0));
      }

  inline uint8_t GetDimensionsCount() const noexcept {return dimensions_count_;}

  inline void SetDimensionsCount(uint8_t count) noexcept {
    dimensions_count_ = count;
  }

 private:
  virtual void Print(std::ostream &stream) const override {
    UnloadCmd::Print(stream);
    stream << "dimensions_count=" << static_cast<int>(dimensions_count_);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const auto &rhs = static_cast<const UnloadArrayCmd&>(cmd);
    return UnloadCmd::IsEqual(cmd)
        && dimensions_count_ == rhs.dimensions_count_;
  }

  uint8_t dimensions_count_;
};

class UnloadIntArrayCmd: public UnloadArrayCmd {
 public:
  using UnloadArrayCmd::UnloadArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadIntArray(*this);
  }
};

class UnloadLongArrayCmd: public UnloadArrayCmd {
 public:
  using UnloadArrayCmd::UnloadArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadLongArray(*this);
  }
};

class UnloadDoubleArrayCmd: public UnloadArrayCmd {
 public:
  using UnloadArrayCmd::UnloadArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadDoubleArray(*this);
  }
};

class UnloadCharArrayCmd: public UnloadArrayCmd {
 public:
  using UnloadArrayCmd::UnloadArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadCharArray(*this);
  }
};

class UnloadStringArrayCmd: public UnloadArrayCmd {
 public:
  using UnloadArrayCmd::UnloadArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadStringArray(*this);
  }
};

class UnloadBoolArrayCmd: public UnloadArrayCmd {
 public:
  using UnloadArrayCmd::UnloadArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitUnloadBoolArray(*this);
  }
};
}
}
#endif
