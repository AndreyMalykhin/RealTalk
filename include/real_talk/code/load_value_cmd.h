
#ifndef _REAL_TALK_CODE_LOAD_VALUE_CMD_H_
#define _REAL_TALK_CODE_LOAD_VALUE_CMD_H_

#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadIntValueCmd: public Cmd {
 public:
  explicit LoadIntValueCmd(int32_t value): value_(value) {}

  void SetValue(int32_t value) {
    value_ = value;
  }

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "value=" << value_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadIntValueCmd &rhs = static_cast<const LoadIntValueCmd &>(cmd);
    return value_ == rhs.value_;
  }

  int32_t value_;
};

class LoadLongValueCmd: public Cmd {
 public:
  explicit LoadLongValueCmd(int64_t value): value_(value) {}

  void SetValue(int64_t value) {
    value_ = value;
  }

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "value=" << value_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadLongValueCmd &rhs = static_cast<const LoadLongValueCmd &>(cmd);
    return value_ == rhs.value_;
  }

  int64_t value_;
};
}
}
#endif
