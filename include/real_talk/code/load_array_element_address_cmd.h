
#ifndef _REAL_TALK_CODE_LOAD_ARRAY_ELEMENT_ADDRESS_CMD_H_
#define _REAL_TALK_CODE_LOAD_ARRAY_ELEMENT_ADDRESS_CMD_H_

#include <cstdint>
#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadArrayElementAddressCmd: public Cmd {
 public:
  inline explicit LoadArrayElementAddressCmd(uint8_t dimensions_count) noexcept
      : dimensions_count_(dimensions_count) {
    assert(dimensions_count != UINT8_C(0));
  }

  void SetDimensionsCount(uint8_t count) {dimensions_count_ = count;}
  uint8_t GetDimensionsCount() const {return dimensions_count_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "dimensions_count=" << static_cast<int>(dimensions_count_);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const auto &rhs = static_cast<const LoadArrayElementAddressCmd&>(cmd);
    return dimensions_count_ == rhs.dimensions_count_;
  }

  uint8_t dimensions_count_;
};

class LoadIntArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadIntArrayElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadIntArrayElementAddress(*this);
  }
};

class LoadLongArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadLongArrayElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadLongArrayElementAddress(*this);
  }
};

class LoadDoubleArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadDoubleArrayElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadDoubleArrayElementAddress(*this);
  }
};

class LoadCharArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadCharArrayElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadCharArrayElementAddress(*this);
  }
};

class LoadBoolArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadBoolArrayElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadBoolArrayElementAddress(*this);
  }
};

class LoadStringArrayElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadStringArrayElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadStringArrayElementAddress(*this);
  }
};
}
}
#endif
