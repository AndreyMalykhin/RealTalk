
#ifndef _REAL_TALK_CODE_LOAD_VALUE_CMD_H_
#define _REAL_TALK_CODE_LOAD_VALUE_CMD_H_

#include <string>
#include <cmath>
#include <limits>
#include "real_talk/code/cmd.h"

namespace real_talk {
namespace code {

class LoadIntValueCmd: public Cmd {
 public:
  explicit LoadIntValueCmd(int32_t value): value_(value) {}
  void SetValue(int32_t value) {value_ = value;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "value=" << value_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadIntValueCmd &rhs = static_cast<const LoadIntValueCmd&>(cmd);
    return value_ == rhs.value_;
  }

  int32_t value_;
};

class LoadLongValueCmd: public Cmd {
 public:
  explicit LoadLongValueCmd(int64_t value): value_(value) {}
  void SetValue(int64_t value) {value_ = value;}

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

class LoadBoolValueCmd: public Cmd {
 public:
  explicit LoadBoolValueCmd(bool value): value_(value) {}
  void SetValue(bool value) {value_ = value;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "value=" << value_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadBoolValueCmd &rhs = static_cast<const LoadBoolValueCmd&>(cmd);
    return value_ == rhs.value_;
  }

  bool value_;
};

class LoadCharValueCmd: public Cmd {
 public:
  explicit LoadCharValueCmd(char value): value_(value) {}
  void SetValue(char value) {value_ = value;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "value=" << value_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadCharValueCmd &rhs = static_cast<const LoadCharValueCmd&>(cmd);
    return value_ == rhs.value_;
  }

  char value_;
};

class LoadStringValueCmd: public Cmd {
 public:
  explicit LoadStringValueCmd(const std::string &value): value_(value) {}
  void SetValue(const std::string &value) {value_ = value;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "value=" << value_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadStringValueCmd &rhs = static_cast<const LoadStringValueCmd&>(cmd);
    return value_ == rhs.value_;
  }

  std::string value_;
};

class LoadDoubleValueCmd: public Cmd {
 public:
  explicit LoadDoubleValueCmd(double value): value_(value) {}
  void SetValue(double value) {value_ = value;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "value=" << value_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadDoubleValueCmd &rhs = static_cast<const LoadDoubleValueCmd&>(cmd);
    return std::fabs(value_ - rhs.value_)
        <= std::numeric_limits<double>::epsilon();
  }

  double value_;
};
}
}
#endif
