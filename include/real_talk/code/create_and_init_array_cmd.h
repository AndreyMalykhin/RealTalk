
#ifndef _REAL_TALK_CODE_CREATE_AND_INIT_ARRAY_CMD_H_
#define _REAL_TALK_CODE_CREATE_AND_INIT_ARRAY_CMD_H_

#include <cassert>
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateAndInitArrayCmd: public Cmd {
 public:
  inline explicit CreateAndInitArrayCmd(
      uint8_t dimensions_count, int32_t values_count) noexcept
      : dimensions_count_(dimensions_count), values_count_(values_count) {
    assert(dimensions_count_ > UINT8_C(0));
  }

  inline uint8_t GetDimensionsCount() const noexcept {
    return dimensions_count_;
  }

  inline void SetDimensionsCount(uint8_t dimensions_count) noexcept {
    dimensions_count_ = dimensions_count;
  }

  inline int32_t GetValuesCount() const noexcept {
    return values_count_;
  }

  inline void SetValuesCount(int32_t values_count) noexcept {
    values_count_ = values_count;
  }

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "dimensions_count=" << static_cast<uint32_t>(dimensions_count_)
           << "; values_count=" << values_count_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const CreateAndInitArrayCmd &rhs =
        static_cast<const CreateAndInitArrayCmd&>(cmd);
    return dimensions_count_ == rhs.dimensions_count_
        && values_count_ == rhs.values_count_;
  }

  uint8_t dimensions_count_;
  int32_t values_count_;
};

class CreateAndInitIntArrayCmd: public CreateAndInitArrayCmd {
 public:
  inline explicit CreateAndInitIntArrayCmd(
      uint8_t dimensions_count, int32_t values_count) noexcept
      : CreateAndInitArrayCmd(dimensions_count, values_count) {}
};

class CreateAndInitLongArrayCmd: public CreateAndInitArrayCmd {
 public:
  inline explicit CreateAndInitLongArrayCmd(
      uint8_t dimensions_count, int32_t values_count) noexcept
      : CreateAndInitArrayCmd(dimensions_count, values_count) {}
};

class CreateAndInitDoubleArrayCmd: public CreateAndInitArrayCmd {
 public:
  inline explicit CreateAndInitDoubleArrayCmd(
      uint8_t dimensions_count, int32_t values_count) noexcept
      : CreateAndInitArrayCmd(dimensions_count, values_count) {}
};

class CreateAndInitBoolArrayCmd: public CreateAndInitArrayCmd {
 public:
  inline explicit CreateAndInitBoolArrayCmd(
      uint8_t dimensions_count, int32_t values_count) noexcept
      : CreateAndInitArrayCmd(dimensions_count, values_count) {}
};

class CreateAndInitCharArrayCmd: public CreateAndInitArrayCmd {
 public:
  inline explicit CreateAndInitCharArrayCmd(
      uint8_t dimensions_count, int32_t values_count) noexcept
      : CreateAndInitArrayCmd(dimensions_count, values_count) {}
};

class CreateAndInitStringArrayCmd: public CreateAndInitArrayCmd {
 public:
  inline explicit CreateAndInitStringArrayCmd(
      uint8_t dimensions_count, int32_t values_count) noexcept
      : CreateAndInitArrayCmd(dimensions_count, values_count) {}
};
}
}
#endif
