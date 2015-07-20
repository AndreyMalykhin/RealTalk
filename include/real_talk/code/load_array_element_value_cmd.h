
#ifndef _REAL_TALK_CODE_LOAD_ARRAY_ELEMENT_VALUE_CMD_H_
#define _REAL_TALK_CODE_LOAD_ARRAY_ELEMENT_VALUE_CMD_H_

#include <cstdint>
#include "real_talk/code/cmd_visitor.h"
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadArrayElementValueCmd: public Cmd {
 public:
  inline explicit LoadArrayElementValueCmd(uint8_t dimensions_count) noexcept
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
    const auto &rhs = static_cast<const LoadArrayElementValueCmd&>(cmd);
    return dimensions_count_ == rhs.dimensions_count_;
  }

  uint8_t dimensions_count_;
};

class LoadArrayOfIntsElementValueCmd: public LoadArrayElementValueCmd {
 public:
  inline explicit LoadArrayOfIntsElementValueCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementValueCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfIntsElementValue(*this);
  }
};

class LoadArrayOfLongsElementValueCmd: public LoadArrayElementValueCmd {
 public:
  inline explicit LoadArrayOfLongsElementValueCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementValueCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfLongsElementValue(*this);
  }
};

class LoadArrayOfDoublesElementValueCmd: public LoadArrayElementValueCmd {
 public:
  inline explicit LoadArrayOfDoublesElementValueCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementValueCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfDoublesElementValue(*this);
  }
};

class LoadArrayOfCharsElementValueCmd: public LoadArrayElementValueCmd {
 public:
  inline explicit LoadArrayOfCharsElementValueCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementValueCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfCharsElementValue(*this);
  }
};

class LoadArrayOfBoolsElementValueCmd: public LoadArrayElementValueCmd {
 public:
  inline explicit LoadArrayOfBoolsElementValueCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementValueCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfBoolsElementValue(*this);
  }
};

class LoadArrayOfStringsElementValueCmd: public LoadArrayElementValueCmd {
 public:
  inline explicit LoadArrayOfStringsElementValueCmd(
      uint8_t dimensions_count) noexcept
      : LoadArrayElementValueCmd(dimensions_count) {}

  virtual void Accept(CmdVisitor *visitor) const override {
    visitor->VisitLoadArrayOfStringsElementValue(*this);
  }
};
}
}
#endif
