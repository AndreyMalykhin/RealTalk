
#ifndef _REAL_TALK_CODE_CREATE_ARRAY_CMD_H_
#define _REAL_TALK_CODE_CREATE_ARRAY_CMD_H_

#include <cassert>
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class CreateArrayCmd: public Cmd {
 public:
  explicit CreateArrayCmd(uint8_t dimensions_count)
      : dimensions_count_(dimensions_count) {
    assert(dimensions_count_ > UINT8_C(0));
  }

  uint8_t GetDimensionsCount() const {
    return dimensions_count_;
  }

  void SetDimensionsCount(uint8_t dimensions_count) {
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
  explicit CreateIntArrayCmd(uint8_t dimensions_count)
      : CreateArrayCmd(dimensions_count) {}
};

class CreateLongArrayCmd: public CreateArrayCmd {
 public:
  explicit CreateLongArrayCmd(uint8_t dimensions_count)
      : CreateArrayCmd(dimensions_count) {}
};

class CreateDoubleArrayCmd: public CreateArrayCmd {
 public:
  explicit CreateDoubleArrayCmd(uint8_t dimensions_count)
      : CreateArrayCmd(dimensions_count) {}
};

class CreateBoolArrayCmd: public CreateArrayCmd {
 public:
  explicit CreateBoolArrayCmd(uint8_t dimensions_count)
      : CreateArrayCmd(dimensions_count) {}
};

class CreateCharArrayCmd: public CreateArrayCmd {
 public:
  explicit CreateCharArrayCmd(uint8_t dimensions_count)
      : CreateArrayCmd(dimensions_count) {}
};

class CreateStringArrayCmd: public CreateArrayCmd {
 public:
  explicit CreateStringArrayCmd(uint8_t dimensions_count)
      : CreateArrayCmd(dimensions_count) {}
};
}
}
#endif
