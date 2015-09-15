
#ifndef _REAL_TALK_VM_STRING_VALUE_H_
#define _REAL_TALK_VM_STRING_VALUE_H_

#include <string>
#include <iostream>

namespace real_talk {
namespace vm {

class StringValue {
 public:
  explicit StringValue(const std::string& str = "");
  explicit StringValue(char c);
  StringValue(const StringValue&) noexcept;
  StringValue(StringValue&&) noexcept;
  ~StringValue();
  void operator=(const StringValue&) noexcept;
  void operator=(StringValue&&) noexcept;
  friend StringValue operator+(const StringValue &lhs, const StringValue &rhs);
  friend bool operator==(const StringValue &lhs, const StringValue &rhs)
      noexcept;
  friend bool operator!=(const StringValue &lhs, const StringValue &rhs)
      noexcept;
  friend std::ostream &operator<<(
      std::ostream &stream, const StringValue &value);

 private:
  class Storage;

  void DecRefsCount() noexcept;

  Storage *storage_;
};
}
}
#endif
