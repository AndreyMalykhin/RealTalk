
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
  inline explicit LoadIntValueCmd(int32_t value) noexcept: value_(value) {}
  inline void SetValue(int32_t value) noexcept {value_ = value;}

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
  inline explicit LoadLongValueCmd(int64_t value) noexcept: value_(value) {}
  inline void SetValue(int64_t value) noexcept {value_ = value;}

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
  inline explicit LoadBoolValueCmd(bool value) noexcept: value_(value) {}
  inline void SetValue(bool value) noexcept {value_ = value;}

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
  inline explicit LoadCharValueCmd(char value) noexcept: value_(value) {}
  inline void SetValue(char value) noexcept {value_ = value;}

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
  inline void SetValue(const std::string &value) {value_ = value;}

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
  inline explicit LoadDoubleValueCmd(double value) noexcept: value_(value) {}
  inline void SetValue(double value) noexcept {value_ = value;}

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

class LoadFuncValueCmd: public Cmd {
 public:
  inline explicit LoadFuncValueCmd(uint32_t address) noexcept
      : address_(address) {}

  inline void SetAddress(uint32_t address) noexcept {address_ = address;}
  inline uint32_t GetAddress() const noexcept {return address_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "address=" << address_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadFuncValueCmd &rhs =
        static_cast<const LoadFuncValueCmd&>(cmd);
    return address_ == rhs.address_;
  }

  uint32_t address_;
};

class LoadNativeFuncValueCmd: public Cmd {
 public:
  inline explicit LoadNativeFuncValueCmd(uint32_t func_index) noexcept
      : func_index_(func_index) {}

  inline void SetFuncIndex(uint32_t func_index) noexcept {
    func_index_ = func_index;
  }

  inline uint32_t GetFuncIndex() const noexcept {return func_index_;}

 private:
  virtual void Print(std::ostream &stream) const override {
    stream << "func_index=" << func_index_;
  }

  virtual bool IsEqual(const Cmd &cmd) const override {
    const LoadNativeFuncValueCmd &rhs =
        static_cast<const LoadNativeFuncValueCmd&>(cmd);
    return func_index_ == rhs.func_index_;
  }

  uint32_t func_index_;
};
}
}
#endif
