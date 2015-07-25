
#ifndef _REAL_TALK_CODE_STORE_CMD_H_
#define _REAL_TALK_CODE_STORE_CMD_H_

#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class StoreCmd: public Cmd {
 protected:
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
  inline explicit StoreArrayCmd(uint8_t dimensions_count) noexcept
      : dimensions_count_(dimensions_count) {
        assert(dimensions_count != UINT8_C(0));
      }

  inline uint8_t GetDimensionsCount() const noexcept {return dimensions_count_;}

  inline void SetDimensionsCount(uint8_t count) noexcept {
    dimensions_count_ = count;
  }

 private:
  virtual void Print(std::ostream &stream) const override {
    StoreCmd::Print(stream);
    stream << "dimensions_count=" << static_cast<int>(dimensions_count_);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const auto &rhs = static_cast<const StoreArrayCmd&>(cmd);
    return StoreCmd::IsEqual(cmd)
        && dimensions_count_ == rhs.dimensions_count_;
  }

  uint8_t dimensions_count_;
};

class StoreIntArrayCmd: public StoreArrayCmd {
 public:
  using StoreArrayCmd::StoreArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreIntArray(*this);
  }
};

class StoreLongArrayCmd: public StoreArrayCmd {
 public:
  using StoreArrayCmd::StoreArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreLongArray(*this);
  }
};

class StoreDoubleArrayCmd: public StoreArrayCmd {
 public:
  using StoreArrayCmd::StoreArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreDoubleArray(*this);
  }
};

class StoreBoolArrayCmd: public StoreArrayCmd {
 public:
  using StoreArrayCmd::StoreArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreBoolArray(*this);
  }
};

class StoreCharArrayCmd: public StoreArrayCmd {
 public:
  using StoreArrayCmd::StoreArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreCharArray(*this);
  }
};

class StoreStringArrayCmd: public StoreArrayCmd {
 public:
  using StoreArrayCmd::StoreArrayCmd;

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitStoreStringArray(*this);
  }
};
}
}
#endif
