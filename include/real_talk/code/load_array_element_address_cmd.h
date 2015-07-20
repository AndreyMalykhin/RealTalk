
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

class LoadArrayOfIntsElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadArrayOfIntsElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfIntsElementAddress(*this);
  }
};

class LoadArrayOfLongsElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadArrayOfLongsElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfLongsElementAddress(*this);
  }
};

class LoadArrayOfDoublesElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadArrayOfDoublesElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfDoublesElementAddress(*this);
  }
};

class LoadArrayOfCharsElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadArrayOfCharsElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfCharsElementAddress(*this);
  }
};

class LoadArrayOfBoolsElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadArrayOfBoolsElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfBoolsElementAddress(*this);
  }
};

class LoadArrayOfStringsElementAddressCmd: public LoadArrayElementAddressCmd {
 public:
  inline explicit LoadArrayOfStringsElementAddressCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementAddressCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfStringsElementAddress(*this);
  }
};
}
}
#endif
