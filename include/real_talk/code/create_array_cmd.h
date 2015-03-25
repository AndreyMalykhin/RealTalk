
#ifndef _REAL_TALK_CODE_CREATE_ARRAY_CMD_H_
#define _REAL_TALK_CODE_CREATE_ARRAY_CMD_H_

#include <cassert>
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateArrayCmd: public Cmd {
 public:
  inline explicit CreateArrayCmd(uint8_t dimensions_count) noexcept
      : dimensions_count_(dimensions_count) {
    assert(dimensions_count_ > UINT8_C(0));
  }

  inline uint8_t GetDimensionsCount() const noexcept {
    return dimensions_count_;
  }

  inline void SetDimensionsCount(uint8_t dimensions_count) noexcept {
    dimensions_count_ = dimensions_count;
  }

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "dimensions_count=" << static_cast<uint32_t>(dimensions_count_);
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const CreateArrayCmd &rhs = static_cast<const CreateArrayCmd&>(cmd);
    return dimensions_count_ == rhs.dimensions_count_;
  }

  uint8_t dimensions_count_;
};

class CreateIntArrayCmd: public CreateArrayCmd {
 public:
  inline explicit CreateIntArrayCmd(uint8_t dimensions_count) noexcept
      : CreateArrayCmd(dimensions_count) {}
};

class CreateLongArrayCmd: public CreateArrayCmd {
 public:
  inline explicit CreateLongArrayCmd(uint8_t dimensions_count) noexcept
      : CreateArrayCmd(dimensions_count) {}
};

class CreateDoubleArrayCmd: public CreateArrayCmd {
 public:
  inline explicit CreateDoubleArrayCmd(uint8_t dimensions_count) noexcept
      : CreateArrayCmd(dimensions_count) {}
};

class CreateBoolArrayCmd: public CreateArrayCmd {
 public:
  inline explicit CreateBoolArrayCmd(uint8_t dimensions_count) noexcept
      : CreateArrayCmd(dimensions_count) {}
};

class CreateCharArrayCmd: public CreateArrayCmd {
 public:
  inline explicit CreateCharArrayCmd(uint8_t dimensions_count) noexcept
      : CreateArrayCmd(dimensions_count) {}
};

class CreateStringArrayCmd: public CreateArrayCmd {
 public:
  inline explicit CreateStringArrayCmd(uint8_t dimensions_count) noexcept
      : CreateArrayCmd(dimensions_count) {}
};
}
}
#endif
